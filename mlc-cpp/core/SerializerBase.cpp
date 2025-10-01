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

        // For each class
        for (auto &clsPtr : m.classes) {
            if (clsPtr->type == "enum" || clsPtr->name == "BaseEnum")
                continue;
            if (clsPtr->name == "DataStorage")
                std::cout << "";
            currentClass = clsPtr.get();
            createSerializationFunction(*currentClass,
                                        SerializationType::SERIALIZATION, name);
            createSerializationFunction(*currentClass,
                                        SerializationType::DESERIALIZATION, name);
        }
    }
}

void SerializerBase::createSerializationFunction(Class &cls,
                                                SerializationType t,
                                                const std::string &format)
{
    // Build method name
    std::string methodName = (t==SerializationType::SERIALIZATION ? "serialize_" : "deserialize_") + format;

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
    fn.is_const     = (t==SerializationType::SERIALIZATION);
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

