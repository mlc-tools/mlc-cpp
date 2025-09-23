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

WriterBase::WriterBase()
  : _model(nullptr)
  , serializer_(nullptr)
{}

void WriterBase::save(Model &model) {
    _model = &model;
    for (auto &cls : model.classes) {
        if (!cls->auto_generated)
            continue;
        if(_model->is_skip(*cls))
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
//    auto ctor = cls->constructor;
//    std::string args, body;
//    if (ctor) {
//        args = createFunctionArgsString(*ctor);
//        body = ctor->bodyText();  // предполагаем метод bodyText()
//    }
//    return { args, body };
    return {"", ""};
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
        assert(0);
//        auto init = serializer_->convertInitialValue(arg.value);
//        auto s = pat
//          .replace("{name}", arg.first)
//          .replace("{type}", type)
//          .replace("{value}", init);
//        parts.push_back(s);
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
        std::regex block(
            "\\{\\{format=" + tag + "\\}\\}[\\s\\S]+?\\{\\{end_format=" + tag + "\\}\\}"
        );
        if ((_model->serializeFormats & all) == all) {
            out = std::regex_replace(out, block, "");
        } else if (!(_model->serializeFormats & code)) {
            out = std::regex_replace(out, block, "");
        } else {
            // leave inner, but strip markers
            out = std::regex_replace(
                out,
                std::regex("\\{\\{format=" + tag + "\\}\\}"),
                ""
            );
            out = std::regex_replace(
                out,
                std::regex("\\{\\{end_format=" + tag + "\\}\\}"),
                ""
            );
        }
    }
    // handle "both"
    std::regex bothBlock("\\{\\{format=both\\}\\}[\\s\\S]+?\\{\\{end_format=both\\}\\}");
    if (_model->serializeFormats != all) {
        out = std::regex_replace(out, bothBlock, "");
    } else {
        out = std::regex_replace(
            out,
            std::regex("\\{\\{format=both\\}\\}"),
            ""
        );
        out = std::regex_replace(
            out,
            std::regex("\\{\\{end_format=both\\}\\}"),
            ""
        );
    }
    return out;
}

std::string WriterBase::prepareFileCodeStylePhp(const std::string &text) const {
    auto out = prepareFile(text);
    int tabs = 0;
    std::ostringstream oss;
    std::istringstream iss(out);
    std::string line;
    while (std::getline(iss, line)) {
        line = std::regex_replace(line, std::regex("^\\s+|\\s+$"), "");
        if (!line.empty() && line[0]=='}') --tabs;
        oss << std::string(tabs, '\t') << line << "\n";
        if (!line.empty() && line[0]=='{') ++tabs;
    }
    // collapse multiple tabs before '{'
    for (int i=0; i<10; ++i) {
        out = oss.str();
        oss.str(""); oss.clear();
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
    int tabs = 0;
    std::ostringstream out;
    std::istringstream iss(text);
    std::string line;

    auto indent = [](int count) {
        if (count <= 0) return std::string();
        return std::string(static_cast<size_t>(count) * 4, ' ');
    };

    while (std::getline(iss, line)) {
        std::string stripped = strip(line);

        bool backward = false;
        if (!stripped.empty() && stripped.front() == '}') {
            --tabs;
        } else if (stripped.find("public:") != std::string::npos ||
                   stripped.find("protected:") != std::string::npos ||
                   stripped.find("private:") != std::string::npos) {
            backward = true;
            --tabs;
        }
        if (tabs < 0) tabs = 0;

        std::string toWrite;
        if (!stripped.empty()) {
            toWrite = indent(tabs) + stripped;
        } // else keep empty line as is
        if (backward) ++tabs;
        if (!stripped.empty() && stripped.front() == '{') ++tabs;

        out << toWrite << '\n';
    }

    std::string prepared = out.str();
//    prepared = std::regex_replace(prepared, std::regex("^\\s+|\\s+$"), "");

    for (;;) {
        auto before = prepared.size();
        replace_all(prepared, "\n\n\n", "\n\n");
        if (prepared.size() == before) break;
    }
//    prepared.push_back('\n');

    return prepared;
}
