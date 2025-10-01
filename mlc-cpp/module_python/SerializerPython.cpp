//
//  SerializerPython.cpp
//

#include "SerializerPython.hpp"
#include "../models/Object.hpp"
#include "../models/Model.hpp"
#include "Common.hpp"
#include <re2/re2.h>
#include "regex.hpp"
#include <set>

SerializerPython::SerializerPython() : SerializerBase() {}
SerializerPython::~SerializerPython() = default;

std::string SerializerPython::getProtocolText(const std::string & /*format*/)
{
    // Python runtime serializers are imported in files; generation just calls them.
    return "";
}

void SerializerPython::createSerializationFunction(Class &cls, SerializationType t, const std::string &format) {
    SerializerBase::createSerializationFunction(cls, t, format);
    if(t == SerializationType::DESERIALIZATION){
        auto method = cls.get_method("deserialize_" + format);
        if(method){
            auto lines = split(method->body, '\n');
            std::set<std::string> imports;
            std::vector<std::string> body;
            body.reserve(lines.size());
            for(auto& line : lines){
                if(line.find("import ") != std::string::npos){
                    if(imports.count(line) == 0)
                        imports.insert(std::move(line));
                }
                else
                    body.push_back(std::move(line));
            }
            method->body.clear();
            for(auto& line : imports)
                method->body += std::move(line) + "\n";
            for(auto& line : body)
                method->body += std::move(line) + "\n";
        }
    }
}

std::string SerializerPython::getParentSerializeCall() const
{
    // Match Python: '        {}.{}(self, {})\n' filled via format_indexes
    return "        {0}.{1}(self, {2})\n";
}

Object SerializerPython::getSerializationFunctionArg(SerializationType t, const std::string & /*format*/) const
{
    (void)t;
    Object arg = Objects::VOID;
    arg.name = "serializer";
    return arg;
}

std::string SerializerPython::buildSerializeOperation(const std::string &fieldName,
                                                      const std::string & fieldType,
                                                      const std::string &fieldValue,
                                                      SerializationType t,
                                                      const std::vector<Object> & tplArgs,
                                                      bool isPointer,
                                                      bool isLink,
                                                      const std::string & /*format*/)
{
    if (t == SerializationType::SERIALIZATION)
    {
        std::string value = fieldValue.empty() ? std::string() : ", " + convertInitializeValue(fieldValue);
        return "\n        serializer.serialize(self." + fieldName + ", \"" + fieldName + "\"" + value + ")";
    }
    // Build meta and local imports for deserialization based on the field itself
    Object temp(fieldType, fieldName);
    temp.template_args = tplArgs;
    temp.is_pointer = isPointer;
    temp.is_link = isLink;
    auto [meta, imports] = create_meta_class(temp);
    std::string imports_text;
    for (auto &imp : imports)
    {
        if (imp == currentClass->name)
            continue;
        imports_text += "        from ." + imp + " import " + imp + "\n";
    }
    std::string defv = fieldValue.empty() ? std::string() : ", " + convertInitializeValue(fieldValue);
    return std::string("\n") + imports_text +
           "        self." + fieldName + " = serializer.deserialize(\"" + fieldName + "\", " + meta + defv + ")";
}

std::string SerializerPython::finalizeSerializeOperation(std::string s) const
{
    // Apply PEP8-like tweaks
    for (auto &pat : RegexPatternPython::pep8)
    {
        if (!pat.pattern)
            continue;
        RE2::GlobalReplace(&s, *pat.pattern, pat.replacement);
    }
    return s;
}

std::string SerializerPython::convertInitializeValue(const std::string &value) const
{
    if (value.empty())
        return value;
    if (value == "true")
        return "True";
    if (value == "false")
        return "False";
    if (value == "nullptr")
        return "None";
    std::string result = value;
    replace_all(result, "::", ".");
    if(result.ends_with('f'))
        result.pop_back();
    return result;
}

std::pair<std::string, std::vector<std::string>> SerializerPython::create_meta_class(const Object& obj) const
{
    static const std::unordered_map<std::string, std::string> types = {
        {"string", "str"},
        {"map", "dict"},
        {"int", "int"},
        {"uint", "int"},
        {"unsigned", "int"},
        {"int64_t", "int"},
        {"uint64_t", "int"},
        {"float", "float"},
        {"double", "float"},
        {"bool", "bool"},
    };
    if (obj.is_link)
    {
        return { "Meta(DataWrapper, " + obj.type + ")", { "DataWrapper", obj.type } };
    }
    else if (obj.is_pointer)
    {
        return { "Meta(IntrusivePtr, " + obj.type + ")", { "IntrusivePtr", obj.type } };
    }
    else if (obj.template_args.empty())
    {
        auto it = types.find(obj.type);
        if (it != types.end())
            return { it->second, {} };
        return { obj.type, { obj.type } };
    }

    std::vector<std::string> meta_args;
    std::vector<std::string> imports_accum;
    for (auto &x : obj.template_args)
    {
        auto [m, imps] = create_meta_class(x);
        meta_args.push_back(m);
        for (auto &q : imps) imports_accum.push_back(q);
    }
    std::string typeStr;
    auto it = types.find(obj.type);
    if (it != types.end()) typeStr = it->second; else typeStr = obj.type;

    std::string meta = "Meta(" + typeStr + ", " + join(meta_args, ',') + ")";
    if (model && model->hasClass(obj.type)) imports_accum.push_back(obj.type);
    return { meta, imports_accum };
}
