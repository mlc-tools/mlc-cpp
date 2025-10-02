//
//  SerializerCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#include "SerializerCpp.hpp"
#include "Common.hpp"
#include "Error.hpp" // for Error::exit
#include "Model.hpp"
#include <cassert>

std::string convert_type(const std::string &type_) {
    if (type_ == "list")
        return "std::vector";
    if (type_ == "map")
        return "std::map";
    if (type_ == "string")
        return "std::string";
    return type_;
}

SerializerCpp::SerializerCpp() : SerializerBase() {}
SerializerCpp::~SerializerCpp() {}

std::string SerializerCpp::getProtocolText(const std::string &format) {
    return "";
}

Object
SerializerCpp::getSerializationFunctionArg(SerializationType t,
                                           const std::string &format) const {
    auto makeObj = [&](const std::string &type, const std::string &name) {
        Object obj;
        obj.type = type;
        obj.name = name;
        obj.is_ref = true;
        return obj;
    };
    if (format == "json") {
        if (t == SerializationType::SERIALIZATION)
            return makeObj("SerializerJson", "serializer");
        else
            return makeObj("DeserializerJson", "deserializer");
    } else {
        if (t == SerializationType::SERIALIZATION)
            return makeObj("SerializerXml", "serializer");
        else
            return makeObj("DeserializerXml", "deserializer");
    }
}

std::string SerializerCpp::buildSerializeOperation(
    const std::string &fieldName, const std::string &fieldType,
    const std::string &fieldValue, SerializationType t,
    const std::vector<Object> &tplArgs, bool isPointer, bool isLink,
    const std::string &format) {
    // Quick-path for primitives with default
    if (t == SerializationType::SERIALIZATION &&
        model->is_simple_type(fieldType) &&
        (!fieldValue.empty() || fieldType == Objects::STRING.type)) {
        return format_indexes(
            "serializer.serialize({0}, \"{0}\", {3}{1}({2}));\n", fieldName,
            fieldType, fieldValue,
            std::string(fieldType == Objects::STRING.type ? "std::" : ""));
    }
    if (t == SerializationType::SERIALIZATION) {
        return format_indexes("serializer.serialize({0}, \"{0}\");\n",
                              fieldName);
    }
    if (t == SerializationType::DESERIALIZATION &&
        model->is_simple_type(fieldType) &&
        (!fieldValue.empty() || fieldType == Objects::STRING.type)) {
        return format_indexes(
            "deserializer.deserialize({0}, \"{0}\", {3}{1}({2}));\n", fieldName,
            fieldType, fieldValue,
            std::string(fieldType == Objects::STRING.type ? "std::" : ""));
    }
    if (t == SerializationType::DESERIALIZATION) {
        return format_indexes("deserializer.deserialize({0}, \"{0}\");\n",
                              fieldName);
    }
    assert(0);
}
