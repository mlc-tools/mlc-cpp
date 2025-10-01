//
//  WriterBase.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "WriterBase.hpp"
#include "Model.hpp"
#include "Class.hpp"
#include "Function.hpp"
#include "Object.hpp"
#include "Common.hpp"
#include <cassert>
#include <sstream>
#include <re2/re2.h>
#include <cstring>
#include <iostream>

WriterBase::WriterBase()
  : _model(nullptr)
  , serializer_(nullptr)
{}

void WriterBase::save(Model &model) {
    _model = &model;
    for (auto &cls : model.classes) {
        if(cls->name == "DataListListBool")
            std::cout << "";
        if (!cls->auto_generated)
            continue;
        if(_model->is_skip(*cls))
            continue;
        if (!model.dirty_classes.empty() && model.dirty_classes.count(cls->name) == 0)
            continue;
        currentClass_ = cls;
        auto sources = writeClass(cls);
        for (auto &p : sources) {
            model.addFile(cls, p.first, p.second);
        }
    }
}

std::vector<std::pair<std::string,std::string>>
WriterBase::writeClass(const std::shared_ptr<Class> & /*cls*/) {
    return {{ "", "" }};
}

void WriterBase::setInitialValues(const std::shared_ptr<Class> &cls) {
    if (cls->type == "enum") {
        for (auto &m : cls->members) {
            if (m.name == "_value" && !m.value.empty()) {
                m.value = cls->members.front().value;
            }
        }
    }
}

std::pair<std::string,std::string>
WriterBase::getConstructorData(const std::shared_ptr<Class> &cls) {
    std::string args, body;
    if (!cls->constructors.empty()) {
        args = createFunctionArgsString(cls->constructors.at(0));
        body = cls->constructors.at(0).body;
    }
    return { args, body };
}

std::pair<std::string,std::string> WriterBase::writeFunction(const Function &method) {
    if (method.name == currentClass_->name)
        return {"", ""};
    auto args = createFunctionArgsString(method);
    auto pattern = getMethodPattern(method);
    auto text = pattern;
    replace_all(text, "{name}", method.name);
    replace_all(text, "{args}", args);
    replace_all(text, "{body}", method.body);
    replace_all(text, "{class_name}", currentClass_->name);
    replace_all(text, "{access}", AccessSpecifierToString(method.access));
    if (method.is_static)
        text = addStaticModifierToMethod(text);
    return {text, ""};
}

std::string WriterBase::createFunctionArgsString(const Function &method) {
    std::vector<std::string> parts;
    if (auto req = getRequiredArgs(method); !req.empty())
        parts.push_back(req);
    for (auto &arg : method.callable_args) {
        auto pat = getMethodArgPattern(arg);
        auto type = getArgumentTypeName(arg);
        auto init = convertInitializeValue(arg.value);
        replace_all(pat, "{name}", arg.name);
        replace_all(pat, "{type}", type);
        replace_all(pat, "{value}", init);
        parts.push_back(pat);
    }
    for (auto &tpl : method.template_args) {
        Object obj;
        obj.name = tpl.name;
        obj.value = getNullptrString();
        auto pat = getMethodArgPattern(obj);
        replace_all(pat, "{name}", tpl.name);
        replace_all(pat, "{value}", getNullptrString());
        parts.push_back(pat);
    }
    std::string res;
    for (size_t i=0;i<parts.size();++i) {
        if (i) res += ", ";
        res += parts[i];
    }
    return res;
}

std::string WriterBase::getArgumentTypeName(const Object &arg) const {
    static const std::map<std::string,std::string> stdTypes = {
        {"string","string"}, {"int","int"}, {"float","float"},
        {"bool","bool"}, {"void",""}
    };
    if (auto it = stdTypes.find(arg.type); it!=stdTypes.end())
        return it->second;
    if (_model->hasClass(arg.type)) return arg.type;
    return "";
}

std::string WriterBase::getMethodArgPattern(const Object &obj) const {
    return obj.value.empty()
      ? "{name}"
      : "{name}={value}";
}

std::string WriterBase::getMethodPattern(const Function & /*method*/) const {
    return "{name}({args})\n{body}";
}

std::string WriterBase::getRequiredArgs(const Function & /*method*/) const {
    return "";
}

std::string WriterBase::getNullptrString() const {
    return "null";
}

std::string WriterBase::addStaticModifierToMethod(const std::string &text) const {
    return "static " + text;
}

std::tuple<std::string,std::string,std::string>
WriterBase::writeObject(const Object & /*obj*/) {
    return {"","",""};
}

std::string WriterBase::prepareFile(const std::string &text) const {
    auto out = text;
    auto all = static_cast<int>(SerializeFormat::Xml) | static_cast<int>(SerializeFormat::Json);

    for (auto fmt : SerializeFormat_getAll()) {
        int code = static_cast<int>(fmt);
        auto tag = SerializeFormat_to_str(fmt);
        RE2 block(
            "\\{\\{format=" + tag + "\\}\\}[\\s\\S]+?\\{\\{end_format=" + tag + "\\}\\}"
        );
        if ((_model->config.serializeFormats & all) == all) {
            RE2::GlobalReplace(&out, block, "");
        } else if (!(_model->config.serializeFormats & code)) {
            RE2::GlobalReplace(&out, block, "");
        } else {
            // leave inner, but strip markers
            RE2::GlobalReplace(&out, RE2("\\{\\{format=" + tag + "\\}\\}"), "");
            RE2::GlobalReplace(&out, RE2("\\{\\{end_format=" + tag + "\\}\\}"), "");
        }
    }
    // handle "both"
    RE2 bothBlock("\\{\\{format=both\\}\\}[\\s\\S]+?\\{\\{end_format=both\\}\\}");
    if (_model->config.serializeFormats != all) {
        RE2::GlobalReplace(&out, bothBlock, "");
    } else {
        RE2::GlobalReplace(&out, RE2("\\{\\{format=both\\}\\}"), "");
        RE2::GlobalReplace(&out, RE2("\\{\\{end_format=both\\}\\}"), "");
    }
    return out;
}

std::string WriterBase::prepareFileCodeStylePhp(const std::string &text) const {
    auto out = prepareFile(text);
    int tabs = 0;
    std::string oss;
    std::istringstream iss(out);
    std::string line;
    while (std::getline(iss, line)) {
        RE2::GlobalReplace(&line, RE2("^\\s+|\\s+$"), "");
        if (!line.empty() && line[0]=='}') --tabs;
        oss += std::string(tabs, '\t');
        oss += line;
        oss.push_back('\n');
        if (!line.empty() && line[0]=='{') ++tabs;
    }
    // collapse multiple tabs before '{'
    for (int i=0; i<10; ++i) {
        out = oss;
        oss.clear();
        std::string pattern = "\n" + std::string(i, '\t') + "{";
        std::string replace = " {";
        size_t pos = 0;
        while ((pos = out.find(pattern, pos)) != std::string::npos) {
            out.replace(pos, pattern.size(), replace);
            pos += replace.size();
        }
    }
    return out;
}

std::string WriterBase::prepareFileCodeStyleCpp(const std::string &text) const {
    // Single-pass formatter: minimal temporaries, controlled allocations.
    int tabs = 0;
    std::string out;
    out.reserve(text.size() + text.size() / 10);

    auto append_indent = [&](int count) {
        if (count <= 0) return;
        out.append(static_cast<size_t>(count) * 4, ' ');
    };

    auto contains_label = [](const char* p, size_t n) -> bool {
        // naive search for public:/protected:/private: inside [p, p+n)
        // avoids constructing std::string
        auto find_sub = [&](const char* sub) {
            size_t m = std::strlen(sub);
            if (m == 0 || m > n) return false;
            for (size_t i = 0; i + m <= n; ++i) {
                if (std::memcmp(p + i, sub, m) == 0) return true;
            }
            return false;
        };
        return find_sub("public:") || find_sub("protected:") || find_sub("private:");
    };

    size_t pos = 0;
    int empty_lines = 0; // keep at most two consecutive empty lines
    while (pos <= text.size()) {
        size_t nl = text.find('\n', pos);
        size_t end = (nl == std::string::npos) ? text.size() : nl; // end is index of '\n' or size()

        // trim spaces on [pos, end)
        size_t l = pos;
        while (l < end && std::isspace(static_cast<unsigned char>(text[l])) && text[l] != '\n') ++l;
        size_t r = end;
        while (r > l && std::isspace(static_cast<unsigned char>(text[r - 1])) && text[r - 1] != '\n') --r;
        size_t len = r - l; // length of trimmed line

        bool backward = false;
        if (len > 0 && text[l] == '}') {
            --tabs;
        } else if (len > 0 && contains_label(text.data() + l, len)) {
            backward = true;
            --tabs;
        }
        if (tabs < 0) tabs = 0;

        if (len == 0) {
            if (empty_lines < 2) {
                out.push_back('\n');
            }
            ++empty_lines;
        } else {
            append_indent(tabs);
            out.append(text.data() + l, len);
            out.push_back('\n');
            empty_lines = 0;
        }

        if (backward) ++tabs;
        if (len > 0 && text[l] == '{') ++tabs;

        if (nl == std::string::npos) break;
        pos = nl + 1;
    }

    return out;
}
