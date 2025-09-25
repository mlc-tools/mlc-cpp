//
//  SerializerBase.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#include "SerializerBase.hpp"
#include "Model.hpp"
#include "Class.hpp"
#include "Function.hpp"
#include "Object.hpp"
#include "Common.hpp"
#include "Error.hpp"    // for Error::exit
#include <cassert>
#include <iostream>

SerializerBase::SerializerBase()
  : model(nullptr), currentClass(nullptr)
{}

SerializerBase::~SerializerBase(){
    
}

void SerializerBase::generateMethods(Model &m) {
    model = &m;
    // For each enabled format, reload protocol and generate methods.
    for (auto &fmt : SerializeFormat_getAll()) {
        int fmtCode = static_cast<int>(fmt);
        const auto &name = SerializeFormat_to_str(fmt);
        if ((m.config.serializeFormats & fmtCode) == 0)
            continue;

        // Load protocol into model.serialize_protocol
        loadDefaultserialize_protocol(name);

        // For each class
        for (auto &clsPtr : m.classes) {
            if (clsPtr->type == "enum" || clsPtr->name == "BaseEnum")
                continue;
            if (clsPtr->name == "DataStorage")
                std::cout << "";
            currentClass = clsPtr.get();
            createSerializationFunction(*currentClass,
                                        SERIALIZATION, name);
            createSerializationFunction(*currentClass,
                                        DESERIALIZATION, name);
        }
    }
}

void SerializerBase::createSerializationFunction(Class &cls,
                                                SerializationType t,
                                                const std::string &format)
{
    // Build method name
    std::string methodName = (t==SERIALIZATION ? "serialize_" : "deserialize_") + format;

    // Check for existing
    if(cls.has_method(methodName))
       return;
    
    if(cls.name == "CloneObject2")
        std::cout << "";
    
    // Prepare new Function
    Function fn;
    fn.name        = methodName;
    fn.return_type  = Objects::VOID;
    fn.is_virtual   = !cls.parent_class_name.empty() || !cls.subclasses.empty();
    fn.is_const     = (t==SERIALIZATION);
    fn.translated  = true;

    // Arg: e.g. (SerializerXml& s) or similar
    auto arg = getSerializationFunctionArg(t, format);
    fn.callable_args.push_back(arg);

    // Body: parent call
    std::string body;
    if (!cls.parent_class_name.empty() &&
        cls.type != "enum" &&
        cls.parent.lock()->type != "enum")
    {
//         use replace_all
        body += format_indexes(
            getParentSerializeCall(),
            cls.parent_class_name,
            methodName,
            fn.callable_args.at(0).name
        );
    }

    // Per-member ops
    for (auto &member : cls.members) {
        if (member.name == "uns_int_value")
            std::cout << "";
        if (member.is_runtime || member.is_static)
            continue;
        if (member.is_const && !member.is_link)
            continue;
        
        if(model->is_skip(member))
            continue;

        body += buildSerializeOperation(
            member.name,
            member.type,
            member.value,
            t,
            member.template_args,
            member.is_pointer,
            member.is_link,
            format
        );
    }

    // Trim trailing newline
    if (!body.empty() && body.back() == '\n') body.pop_back();
    fn.body = body;

    cls.functions.push_back(std::move(fn));
}

std::string SerializerBase::dispatchSerializeOp(
    const std::string &fieldName,
    const std::string &fieldType,
    const std::string &fieldValue,
    SerializationType t,
    const std::vector<Object> &templateArgs,
    bool isPointer,
    bool isLink,
    const std::string &format)
{
    // Determine index: default vs with default value
    int idx = fieldValue.empty() ? 1 : 0;

    // Determine protocol key type
    std::string keyType = fieldType;
    auto clsPtr = model->hasClass(fieldType)
                ? model->get_class(fieldType)
                : nullptr;
    if (clsPtr && clsPtr->type == "enum") {
        keyType = "enum";
    }
    else if (!model->is_simple_type(fieldType) && keyType!="list" && keyType!="map")
    {
        if (isLink)
            keyType = "link";
        else if (isPointer)
            keyType = "pointer";
        else
            keyType = "serialized";
    }
    else if (model->is_simple_type(fieldType)) {
        keyType = fieldType;
    }
    else {
        // list or map templates
        if (keyType=="map") {
            return buildMapSerialization(fieldName, templateArgs, t, format);
        }
        // list<T>
        auto &arg = templateArgs.front();
        if (arg.is_link)
            keyType = "list<link>";
        else if (model->is_simple_type(arg.type))
            keyType = "list<{" + arg.type + "}>";
        else if (arg.is_pointer)
            keyType = "list<pointer>";
        else if (model->hasClass(arg.type) && model->get_class(arg.type)->type=="enum") {
            keyType = "list<string>";
        }
        else keyType = "list<serialized>";
    }

    // Convert default value
    std::string defv = convertInitializeValue(fieldValue);

    // Lookup in protocol
    auto &vec = serialize_protocol[t].at(keyType);
    std::string pattern = vec.at(idx);

    replace_all(pattern, "{field}",         fieldName);
    replace_all(pattern, "{type}",          fieldType);
    replace_all(pattern, "{default_value}", defv);
    replace_all(pattern, "{owner}",         currentClass->name);
    replace_all(pattern, "{arg_0}",         templateArgs.empty() ? "" : templateArgs[0].type);
    replace_all(pattern, "{format}",        format);

    return finalizeSerializeOperation(std::move(pattern));
}

void SerializerBase::loadDefaultserialize_protocol(const std::string& formatName){
//    serialize_protocol = m.serialize_protocol;
}
