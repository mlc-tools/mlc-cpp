//
//  WriterPython.cpp
//

#include "WriterPython.hpp"

#include "../core/SerializerBase.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Model.hpp"
#include "../models/Object.hpp"
#include "Common.hpp"
#include "SerializerPython.hpp"

#include <iostream>
#include <sstream>

const std::string PATTERN_FILE = R"__(
# -*- coding: utf-8 -*-
import xml.etree.ElementTree as ET
import json
from .mg_extensions import *
{imports}
class {name}:
{static_list}
    {slots}
    def __init__({constructor_args}):
{init_superclass}
{initialize_list}
{constructor_body}
    def __hash__(self):
        return id(self)
{functions}
)__";

const std::string PATTERN_METHOD = R"__(
    def {name}({args}):
{body}
)__";

WriterPython::WriterPython() : WriterBase() {}

std::vector<std::pair<std::string, std::string>>
WriterPython::writeClass(const std::shared_ptr<Class> &cls) {
    if (cls->name == "ConstructableClassWithParameters")
        std::cout << "";
    currentClass_ = cls;
    setInitialValues(cls);

    std::string imports_in_static;
    std::string imports_in_ctor;
    std::string initialize_list;
    std::string static_list;
    std::vector<std::string> slots;
    for (auto &obj : cls->members) {
        if (_model->is_skip(obj))
            continue;
        auto [imports, init, staticInit] = writeObject(obj);
        if (!obj.is_static) {
            if (!imports.empty())
                imports_in_ctor += imports;
            if (!init.empty()) {
                initialize_list += "        " + init + "\n";
            }
            slots.push_back("\"" + obj.name + "\"");
        } else {
            imports_in_static += imports;
            if (!staticInit.empty()) {
                static_list += "    " + staticInit + "\n";
            }
        }
    }
    initialize_list = imports_in_ctor + initialize_list;
    if (initialize_list.empty())
        initialize_list = "        pass";

    std::string slots_line = "__slots__ = [";
    for (size_t i = 0; i < slots.size(); ++i) {
        slots_line += slots[i];
        if (i + 1 < slots.size())
            slots_line += ", ";
    }
    slots_line += "]";

    std::string functions;
    for (auto &method : cls->functions) {
        if (_model->is_skip(method))
            continue;
        auto p = writeFunction(method);
        functions += p.second;
    }

    std::string imports = "from .common import *\n";
    if (cls->type != "enum" && cls->name != "BaseEnum") {
        imports += "from .SerializerXml import SerializerXml\n";
        imports += "from .DeserializerXml import DeserializerXml\n";
        imports += "from .SerializerJson import SerializerJson\n";
        imports += "from .DeserializerJson import DeserializerJson\n";
        imports += "from .IntrusivePtr import *\n";
        imports += "from .Meta import *\n";
    }

    std::string init_superclass;
    std::string name = cls->name;
    std::string superclass_name = "object";
    if (!cls->parent_class_name.empty()) {
        superclass_name = cls->parent_class_name;
        imports +=
            "from ." + superclass_name + " import " + superclass_name + "\n";
        init_superclass = "        " + superclass_name + ".__init__(self)";
    }
    name += "(" + superclass_name + ")";

    std::string constructor_body;
    auto [ctor_args, ctor_body] = getConstructorData(cls);
    constructor_body += ctor_body;
    if (ctor_args.empty())
        ctor_args = "self";

    std::string result = PATTERN_FILE;
    replace_all(result, "{name}", name);
    replace_all(result, "{initialize_list}", initialize_list);
    replace_all(result, "{functions}", functions);
    replace_all(result, "{imports}", imports);
    replace_all(result, "{init_superclass}", init_superclass);
    replace_all(result, "{static_list}", static_list);
    replace_all(result, "{slots}", slots_line);
    replace_all(result, "{constructor_args}", ctor_args);
    replace_all(result, "{constructor_body}", constructor_body);

    std::string text = preparePythonFile(result);
    return {{cls->name + ".py", prepareFile(text)}};
}

// [imports, initialization, static_initialization]
std::tuple<std::string, std::string, std::string>
WriterPython::writeObject(const Object &obj) {
    Object o = obj;
    if (o.name == "from")
        o.name = "from_";
    std::string imports;
    std::string value = convertInitializeValue(o.value);
    if (value.empty() && !o.is_pointer) {
        const std::string &type_ = o.type;
        if (type_ == "string")
            value = "\"\"";
        if (type_ == "int")
            value = "0";
        if (type_ == "float")
            value = "0";
        if (type_ == "uint" || type_ == "unsigned")
            value = "0";
        if (type_ == "bool")
            value = "False";
        if (type_ == "list")
            value = "[]";
        if (type_ == "map")
            value = "{}";
        if (value.empty()) {
            if (_model->hasClass(o.type) && currentClass_->name != o.type)
                value = o.type + "()";
        }
    }
    if (_model->hasClass(o.type) && currentClass_->name != o.type &&
        o.type != currentClass_->parent_class_name)
        imports += "        from ." + obj.type + " import " + obj.type + "\n";

    if (!value.empty() && value.size() >= 1 && value.back() == 'f') {
        value.pop_back();
        value += '0';
    }

    if (value.empty() && o.is_pointer)
        value = "None";

    std::string line;
    if (o.is_static)
        line = o.name + " = " + value;
    else
        line = "self." + o.name + " = " + value;

    return {
        imports,
        o.is_static ? "" : line,
        !o.is_static ? "" : line,
    };
}

std::pair<std::string, std::string>
WriterPython::writeFunction(const Function &fn) {
    // Build pythonic args: 'self' unless static, then each arg name or
    // name=default
    std::vector<std::string> parts;
    if (!fn.is_static)
        parts.push_back("self");
    for (auto &a : fn.callable_args) {
        std::string one = a.name;
        if (!a.value.empty())
            one += "=" + a.value;
        parts.push_back(std::move(one));
    }
    for (auto &t : fn.template_args) {
        parts.push_back(t.type + std::string("=") + getNullptrString());
    }
    std::string args;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i)
            args += ", ";
        args += parts[i];
    }
    std::string body;
    if (fn.body.empty())
        body += "        pass\n";
    else
        body += fn.body +
                (fn.body.back() == '\n' ? std::string("") : std::string("\n"));
    body += "\n        pass\n";
    std::string method;
    method += ::format_indexes("    def {0}({1}):\n", fn.name, args);
    method += body + "\n";
    return {"", method};
}

std::string WriterPython::getMethodArgPattern(const Object &arg) const {
    return arg.value.empty() ? "{name}" : "{name}={value}";
}

std::string WriterPython::getMethodPattern(const Function & /*fn*/) const {
    return R"(
    def {name}({args}):
{body}
    )";
}

std::string WriterPython::getRequiredArgs(const Function &method) const {
    if (!method.is_static)
        return "self";
    return "";
}

std::string WriterPython::getNullptrString() const { return "None"; }

std::string
WriterPython::addStaticModifierToMethod(const std::string &text) const {
    return std::string("    @staticmethod\n") + text;
}

std::string WriterPython::preparePythonFile(const std::string &text) const {
    // Mirror Python writer's prepare_file spacing rules
    std::vector<std::string> lines;
    {
        std::string cur;
        cur.reserve(text.size());
        for (char c : text) {
            if (c == '\n') {
                lines.push_back(std::move(cur));
                cur.clear();
            } else {
                cur.push_back(c);
            }
        }
        // keep trailing line even if no final newline
        if (!cur.empty())
            lines.push_back(cur);
    }

    auto starts_with = [](const std::string &s, const char *p) -> bool {
        size_t n = std::char_traits<char>::length(p);
        return s.size() >= n && s.compare(0, n, p) == 0;
    };
    auto lstrip_starts_with = [](const std::string &s, const char *p) -> bool {
        size_t i = 0;
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t'))
            ++i;
        size_t n = std::char_traits<char>::length(p);
        return s.size() - i >= n && s.compare(i, n, p) == 0;
    };
    auto is_blank = [](const std::string &s) -> bool {
        for (char c : s)
            if (c != ' ' && c != '\t' && c != '\r')
                return false;
        return true;
    };

    std::vector<std::string> result;
    bool tabs = false;
    bool is_static = false;
    for (const auto &line : lines) {
        if (is_blank(line)) {
            continue;
        }
        if (!starts_with(line, "    ") && tabs) {
            result.emplace_back("");
            result.emplace_back("");
        } else if (starts_with(line, "    @")) {
            is_static = true;
            result.emplace_back("");
            result.emplace_back(line);
            continue;
        } else if (!is_static) {
            if (starts_with(line, "class ") || starts_with(line, "def ")) {
                result.emplace_back("");
                result.emplace_back("");
            } else if (lstrip_starts_with(line, "def ")) {
                result.emplace_back("");
            }
        }
        result.emplace_back(line);
        if (starts_with(line, "    ")) {
            tabs = true;
        }
        is_static = false;
    }
    result.emplace_back("");

    std::string out;
    // Join with newlines
    for (size_t i = 0; i < result.size(); ++i) {
        out += result[i];
        if (i + 1 < result.size())
            out.push_back('\n');
    }
    return out;
}

std::string WriterPython::convertInitializeValue(const std::string &value) {
    return SerializerPython().convertInitializeValue(value);
}
