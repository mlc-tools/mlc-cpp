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
#include <re2/re2.h>
#include <sstream>

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
        method.body = translateFunctionBody(cls, method, body, model,
                                            method.callable_args);
    } else {
        if (!method.body.empty()) {
            return method.body;
            //            // Ensure method.body lines are indented by 8 spaces
            //            inside def std::string indented =
            //            indent_lines(method.body, 8); if (!indented.empty() &&
            //            indented.back() != '\n')
            //                indented.push_back('\n');
            //            method.body = indented;
        } else {
            method.body = "        pass\n";
        }
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
    out = addImports(cls, out, model);
    out = convertBraces(out);
    return out;
}

std::string
TranslatorPython::replaceByRegex(const std::string &body, Class & /*cls*/,
                                 Function & /*method*/, Model & /*model*/,
                                 const std::vector<Object> & /*args*/) {
    if (body.empty())
        return body;
    // Save string literals to avoid touching code inside quotes
    auto saved = saveStrings(body);
    std::string out = saved.first;

    // Apply function patterns
    for (auto &pat : RegexPatternPython::functionPatterns) {
        replacePattern(out, pat);
    }
    // Simple replaces
    for (auto &pr : RegexPatternPython::replaces) {
        replace_all(out, pr.first, pr.second);
    }

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
        if (func.find(c.name) != std::string::npos) {
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
    bool next_tab = false;
    auto ltrim = [](const std::string &s) {
        size_t i = 0;
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t'))
            ++i;
        return s.substr(i);
    };
    for (size_t i = 0; i < lines.size(); ++i) {
        std::string line = lines[i];
        if (next_tab)
            tabs += 1;
        size_t pos;
        if ((pos = line.find('{')) != std::string::npos) {
            tabs += 1;
            line.erase(pos, 1);
        }
        if ((pos = line.find('}')) != std::string::npos) {
            tabs -= 1;
            line.erase(pos, 1);
        }
        std::string stripped = ltrim(line);
        lines[i] = getTabs(tabs) + stripped;
        if (next_tab) {
            next_tab = false;
            tabs -= 1;
        }
        bool starts_ctl = (stripped.rfind("for", 0) == 0) ||
                          (stripped.rfind("if", 0) == 0) ||
                          (stripped.rfind("else", 0) == 0) ||
                          (stripped.rfind("elif", 0) == 0) ||
                          (stripped.rfind("try:", 0) == 0) ||
                          (stripped.rfind("except", 0) == 0);
        if (starts_ctl && i + 1 < lines.size() &&
            line.find('{') == std::string::npos) {
            // trigger one-line block indentation on next line when no braces
            next_tab = true;
        }
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

std::vector<int> TranslatorPython::convertToEnum(Class &cls) {
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

    return values;
}
