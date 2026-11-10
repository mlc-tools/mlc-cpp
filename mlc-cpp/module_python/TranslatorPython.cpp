//
//  TranslatorPython.cpp
//

#include "TranslatorPython.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Model.hpp"
#include "../models/Object.hpp"

#include "../utils/Common.hpp"
#include "regex.hpp"
#include <iostream>
#include <cctype>
#include <re2/re2.h>
#include <sstream>
#include <set>

// Translate calls with balanced arguments before the legacy regex rules.
static std::string translateTemplateCalls(std::string text) {
    RE2 call(R"(\b(\w+)<([\w:, *&]+)>\()");
    re2::StringPiece rest(text);
    std::string name, types;
    size_t offset = 0;
    while (RE2::FindAndConsume(&rest, call, &name, &types)) {
        size_t open = text.size() - rest.size() - 1;
        size_t start = open - name.size() - types.size() - 2;
        size_t end = open + 1;
        int depth = 1;
        for (; end < text.size() && depth; ++end) {
            if (text[end] == '(') ++depth;
            if (text[end] == ')') --depth;
        }
        if (depth) break;
        auto args = translateTemplateCalls(text.substr(open + 1, end - open - 2));
        std::string replacement;
        if (name == "min" || name == "max") {
            replacement = name + "(" + args + ")";
        } else if (name == "static_cast") {
            if (types == "int64_t" || types == "unsigned" || types == "uint") types = "int";
            replacement = types + "(" + args + ")";
        } else if (name == "dynamic_pointer_cast_intrusive") {
            types = types.substr(0, types.find(','));
            replacement = "dynamic_pointer_cast_intrusive(" + args + ", " + types + ")";
        } else {
            replacement = name + "(" + args + (args.empty() ? "" : ", ") + types + ")";
        }
        text.replace(start, end - start, replacement);
        offset = start + replacement.size();
        rest = re2::StringPiece(text.data() + offset, text.size() - offset);
    }
    return text;
}

// Recursively translate conditional expressions, retaining nested call boundaries.
static std::string translateConditional(std::string text) {
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] != '(' && text[i] != '[') continue;
        char close = text[i] == '(' ? ')' : ']';
        int depth = 1;
        size_t end = i + 1;
        for (; end < text.size() && depth; ++end) {
            if (text[end] == text[i]) ++depth;
            if (text[end] == close) --depth;
        }
        if (depth) continue;
        auto inner = translateConditional(text.substr(i + 1, end - i - 2));
        text.replace(i + 1, end - i - 2, inner);
        i += inner.size() + 1;
    }
    int depth = 0;
    size_t question = std::string::npos, colon = std::string::npos;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '(' || text[i] == '[') ++depth;
        if (text[i] == ')' || text[i] == ']') --depth;
        if (depth) continue;
        if (text[i] == '?' && question == std::string::npos) question = i;
        else if (text[i] == ':' && question != std::string::npos) { colon = i; break; }
    }
    if (colon == std::string::npos) return text;
    size_t start = 0;
    auto assignment = text.find(" = ");
    if (assignment < question) start = assignment + 3;
    else if (text.find("return ") < question) start = text.find("return ") + 7;
    return text.substr(0, start) + "(" + text.substr(question + 1, colon - question - 1) +
        " if " + text.substr(start, question - start) + " else " + text.substr(colon + 1) + ")";
}

inline std::string indent_lines(const std::string &s, int spaces) {
    std::string out;
    std::string pad(spaces, ' ');
    size_t i = 0;
    while (i < s.size()) {
        size_t j = s.find('\n', i);
        if (j == std::string::npos) {
            out += pad;
            out.append(s, i, std::string::npos);
            break;
        }
        out += pad;
        out.append(s, i, j - i);
        out.push_back('\n');
        i = j + 1;
    }
    return out;
}

void TranslatorPython::translateFunction(Class &cls, Function &method,
                                         Model &model) {
    if (cls.name == "DataStorage" && method.name == "initialize_json")
        std::cout << "";

    // Match Python translator: generate empty methods when requested
    if (model.config.empty_methods && cls.name != "DataStorage" &&
        method.name != "get_type") {
        method.body = "        pass\n";
        return;
    }

    if (!method.translated) {
        std::string body = method.body;
        if (body.find("@(get_components)") != std::string::npos) {
            body.clear();
            for (const auto &component : model.classes) {
                bool skill = false;
                for (auto base = component->parent.lock(); base; base = base->parent.lock())
                    if (base->name == "ComponentSkillBase") skill = true;
                if (!skill || !component->subclasses.empty()) continue;
                std::string block = method.body;
                replace_all(block, "@(get_components)", "get_components<" + component->name + ">()");
                replace_all(block, "@(components)", component->name);
                body += block + "\n";
            }
        }
        method.body = translateFunctionBody(cls, method, body, model,
                                            method.callable_args);
    } else if (method.body.empty()) {
            method.body = "        pass\n";
    }
}

std::string
TranslatorPython::translateFunctionBody(Class &cls, Function &method,
                                        const std::string &body, Model &model,
                                        const std::vector<Object> &args) {
    if (method.name == "test_while_simple")
        std::cout << "";
    std::string out =
        TranslatorBase::translateFunctionBody(cls, method, body, model, args);
    if (out.empty())
        out = "pass";
    //    out = replaceByRegex(out, cls, method, model, args);
    out = convertBracesToTabs(out);
    out = removeDoubleEol(out);
    out = convertBraces(out);
    return out;
}

std::string
TranslatorPython::replaceByRegex(const std::string &body, Class &cls,
                                 Function &method, Model &model,
                                 const std::vector<Object> &args) {
    if (body.empty())
        return body;
    // Save string literals to avoid touching code inside quotes
    auto saved = saveStrings(body);
    std::string out = saved.first;

    // C++ permits implicit member access; Python requires an explicit owner.
    std::set<std::string> locals;
    for (const auto &arg : args)
        locals.insert(arg.name);
    re2::StringPiece declarations(out);
    RE2 declaration(R"(\b([\w:]+)(?:<[^;\n]+>)?\s*[*&]*\s+(\w+)\s*(?:=|;|:|\)))");
    std::string type, name;
    while (RE2::FindAndConsume(&declarations, declaration, &type, &name)) {
        if (type == "auto" || type == "list" || type == "map" || type == "hash_map" ||
            type == "set" || type == "hash_set" || Model::simpleTypes.count(type) ||
            model.hasClass(type) || model.hasClass(cls.name + type))
            locals.insert(name);
    }
    re2::StringPiece bindings(out);
    std::string first, second;
    RE2 binding(R"(auto\s*&*\s*\[(\w+),\s*(\w+)\])");
    while (RE2::FindAndConsume(&bindings, binding, &first, &second)) {
        locals.insert(first);
        locals.insert(second);
    }
    re2::StringPiece lambdas(out);
    RE2 lambda(R"(\((\w+)(?:,\s*(\w+))?\s*:>)");
    while (RE2::FindAndConsume(&lambdas, lambda, &first, &second)) {
        locals.insert(first);
        locals.insert(second);
    }
    for (const Class *owner = &cls; owner; owner = owner->parent.lock().get()) {
        for (const auto &function : owner->functions) {
            if (function.name.empty() || locals.count(function.name)) continue;
            RE2 reference("(^|[^\\w.>:])\\b" + function.name + "\\s*\\(");
            auto prefix = function.is_static ? cls.name + "::" : "self.";
            if (!method.is_static || function.is_static)
                RE2::GlobalReplace(&out, reference, "\\1" + prefix + function.name + "(");
        }
        for (const auto &member : owner->members) {
            if (member.name.empty() || locals.count(member.name))
                continue;
            RE2 reference("(^|[^\\w.>:])\\b" + member.name + "\\b");
            const auto prefix = member.is_static ? cls.name + "::" : "self.";
            if (!method.is_static || member.is_static)
                RE2::GlobalReplace(&out, reference, "\\1" + prefix + member.name);
        }
    }

    RE2::GlobalReplace(&out, RE2(R"(\(void\)\w+;)"), "pass;");
    RE2::GlobalReplace(&out, RE2(R"(std::numeric_limits<unsigned>::max\(\))"), "4294967295");
    RE2::GlobalReplace(&out, RE2(R"(\breturn\s+([\w.]+)\s*=\s*([^=;][^;]*);)"), "\\1 = \\2;\nreturn \\1;");
    RE2::GlobalReplace(&out, RE2(R"(\b[\w:]+\s*\*\s*(\w+)\s*=)"), "\\1 =");
    // Preserve DataStorage's specialized getter before generic template calls.
    RE2::GlobalReplace(&out, RE2(R"(DataStorage::shared\(\).get<(\w+)>)"), "DataStorage::shared().get\\1");
    RE2::GlobalReplace(&out, RE2(R"(std::sort\((\w+)\.begin\(\),\s*\w+\.end\(\),\s*\[\]\(\w+\s*[*&]*\s+(\w+),\s*\w+\s*[*&]*\s+\w+\)\s*\{\s*return\s+\w+->(.+?)\s*>\s*[^;]+;\s*\}\))"),
                       "\\1.sort(key=lambda \\2: \\2.\\3, reverse=True)");
    out = translateTemplateCalls(out);

    // Apply function patterns
    for (auto &pat : RegexPatternPython::functionPatterns) {
        replacePattern(out, pat);
    }
    // Simple replaces
    for (auto &pr : RegexPatternPython::replaces) {
        replace_all(out, pr.first, pr.second);
    }

    std::istringstream conditionalLines(out);
    std::string line, conditionalResult;
    while (std::getline(conditionalLines, line))
        conditionalResult += translateConditional(line) + "\n";
    out = conditionalResult;

    // Restore literals
    out = restoreStrings(out, saved.second);
    return out;
}

std::string TranslatorPython::addImports(const Class &clsOwner,
                                         const std::string &func,
                                         const Model &model) {
    if (func.empty())
        return func;
    std::vector<std::string> imports;
    if (func.find("Factory") != std::string::npos) {
        imports.emplace_back("from .Factory import Factory");
    }

    for (const auto &cptr : model.classes) {
        const Class &c = *cptr;
        if (c.name == clsOwner.name)
            continue;
        auto isIdentifier = [](char ch) {
            return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
        };
        bool referenced = false;
        for (size_t pos = func.find(c.name); pos != std::string::npos;
             pos = func.find(c.name, pos + c.name.size())) {
            const auto end = pos + c.name.size();
            if ((pos == 0 || !isIdentifier(func[pos - 1])) &&
                (end == func.size() || !isIdentifier(func[end]))) {
                referenced = true;
                break;
            }
        }
        if (referenced) {
            imports.emplace_back("from ." + c.name + " import " + c.name);
        }
    }
    if (func.find("math.") != std::string::npos)
        imports.emplace_back("import math");
    if (func.find("random.") != std::string::npos)
        imports.emplace_back("import random");

    if (imports.empty())
        return func;
    std::string out;
    std::string pad = getTabs(2);
    for (size_t i = 0; i < imports.size(); ++i) {
        if (func.find(imports[i] + "\n") != std::string::npos)
            continue;
        out += pad;
        out += imports[i];
        out.push_back('\n');
    }
    out += func;
    return out;
}

std::string TranslatorPython::removeDoubleEol(const std::string &func) {
    if (func.empty())
        return func;
    std::string out = func;
    auto rep = [&](const std::string &from, const std::string &to) {
        size_t p = 0;
        while ((p = out.find(from, p)) != std::string::npos) {
            out.replace(p, from.size(), to);
        }
    };
    rep("\n    \n", "\n");
    rep("\n        \n", "\n");
    rep("\n            \n", "\n");
    rep("\n                \n", "\n");
    rep("\n                    \n", "\n");
    rep("\n                        \n", "\n");
    return out;
}

std::string TranslatorPython::convertBracesToTabs(const std::string &func) {
    if (func.find("js = json.loads(content)") != std::string::npos)
        std::cout << "";
    if (func.empty())
        return func;
    std::vector<std::string> lines;
    {
        std::string cur;
        for (char c : func) {
            if (c == '\n') {
                lines.push_back(cur);
                cur.clear();
            } else {
                cur.push_back(c);
            }
        }
        if (!cur.empty())
            lines.push_back(cur);
    }

    int tabs = 2;
    std::vector<bool> blocks;
    bool control = false;
    bool single = false;
    for (auto &line : lines) {
        auto start = line.find_first_not_of(" \t\r");
        std::string stripped = start == std::string::npos ? "" : line.substr(start);
        auto last = stripped.find_last_not_of(" \t\r");
        if (last != std::string::npos) stripped.resize(last + 1);
        if (stripped.empty()) { line.clear(); continue; }
        if (stripped == "{") {
            blocks.push_back(control);
            if (control) ++tabs;
            control = false;
            line.clear();
            continue;
        }
        if (stripped == "}") {
            if (!blocks.empty()) {
                if (blocks.back()) --tabs;
                blocks.pop_back();
            }
            control = false;
            line.clear();
            continue;
        }
        single = control;
        line = getTabs(tabs + (single ? 1 : 0)) + stripped;
        control = stripped.back() == ':' &&
            (stripped.rfind("for ", 0) == 0 || stripped.rfind("if ", 0) == 0 ||
             stripped.rfind("elif ", 0) == 0 || stripped.rfind("else:", 0) == 0 ||
             stripped.rfind("while", 0) == 0 || stripped.rfind("try:", 0) == 0 ||
             stripped.rfind("except ", 0) == 0);
    }
    std::string out;
    for (size_t i = 0; i < lines.size(); ++i) {
        out += lines[i];
        if (i + 1 < lines.size())
            out.push_back('\n');
    }
    return out;
}

std::string TranslatorPython::convertBraces(const std::string &body) {
    std::string out = body;
    size_t p = 0;
    while ((p = out.find("[@[", p)) != std::string::npos) {
        out.replace(p, 3, "{");
        p += 1;
    }
    p = 0;
    while ((p = out.find("]@]", p)) != std::string::npos) {
        out.replace(p, 3, "}");
        p += 1;
    }
    return out;
}

std::string TranslatorPython::getTabs(int count) {
    if (count <= 0)
        return std::string();
    return std::string(static_cast<size_t>(count) * 4, ' ');
}

void TranslatorPython::convertToEnum(Class &cls) {
    TranslatorBase::convertToEnum(cls);
    // Ported from py/translator.py: create _value and helper methods
    std::vector<int> values;
    std::string cast =
        cls.members.empty() ? std::string("string") : cls.members.back().type;

    Object member;
    member.name = "_value";
    member.type = "string";
    cls.members.push_back(member);

    // setter
    Function setter;
    setter.name = "set";
    // arg 'value' (type not used in python emitter)
    setter.callable_args.push_back(Object("", "value"));

    if (cast == "int") {
        cls.members.back().type = "int";
        for (auto &obj : cls.members) {
            if (obj.name == "_value")
                continue;
            setter.body += ::format_indexes(
                R"(if(value == "{0}")
{
    this->_value = {1}::{0};
    return;
})",
                obj.name, cls.name);
            setter.body += "\n";
        }
    } else {
        setter.body = "this->_value = value;\n";
    }
    cls.functions.push_back(setter);

    // getter
    Function getter;
    getter.name = "str";
    getter.return_type.type = "string";
    if (cast == "int") {
        for (auto &obj : cls.members) {
            if (obj.name == "_value")
                continue;
            getter.body += ::format_indexes(
                R"(if(this->_value == {0}::{1})
{
    return "{1}";
})",
                cls.name, obj.name);
            getter.body += "\n";
        }
    } else {
        getter.body = "return this->_value;\n";
    }
    cls.functions.push_back(getter);

    auto add_binary_method = [&](const std::string &name,
                                 const std::string &op) {
        Function m;
        m.name = name;
        m.return_type.type = "bool";
        m.callable_args.push_back(Object("", "rhs"));
        m.body = ::format_indexes(
            R"(if(isinstance(rhs, {0}))
{
    return this->_value {1} rhs._value;
}
else
{
    return this->_value {1} rhs
})",
            cls.name, op);
        cls.functions.push_back(m);
    };
    add_binary_method("__eq__", "==");
    add_binary_method("__and__", "&");
    add_binary_method("__or__", "|");
    add_binary_method("__xor__", "^");
}
