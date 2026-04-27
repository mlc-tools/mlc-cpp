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


SerializerCpp::SerializerCpp() : SerializerBase() {}
SerializerCpp::~SerializerCpp() {}

std::string SerializerCpp::getProtocolText(const std::string &format) {
    return "";
}

Object
SerializerCpp::getSerializationFunctionArg(SerializationType t,
                                           const std::string &format) const {
    auto makeObj = [&](const std::string &type, const std::string &name, bool is_ref) {
        Object obj;
        obj.type = type;
        obj.name = name;
        obj.is_ref = is_ref;
        return obj;
    };
    if (format == "json") {
        if (t == SerializationType::SERIALIZATION)
            return makeObj("Json::Value", "node", true);
        else
            return makeObj("Json::Value", "node", true);
    } else {
        if (t == SerializationType::SERIALIZATION)
            return makeObj("pugi::xml_node", "node", false);
        else
            return makeObj("pugi::xml_node", "node", false);
    }
}

std::string SerializerCpp::buildSerializeOperation(
    const std::string &fieldName, const std::string &fieldType,
    const std::string &fieldValue, SerializationType t,
    const std::vector<Object> &tplArgs, bool isPointer, bool isLink,
    const std::string &format) {
    // Quick-path for primitives with default
    if (t == SerializationType::SERIALIZATION && model->is_simple_type(fieldType) && (!fieldValue.empty() || fieldType == Objects::STRING.type)) {
        return format_indexes(
            "serializer_{4}::serialize(node, {0}, \"{0}\", {3}{1}({2}));\n", fieldName,
            fieldType, fieldValue,
            std::string(fieldType == Objects::STRING.type ? "std::" : ""), format);
    }
    if (t == SerializationType::SERIALIZATION) {
        return format_indexes("serializer_{1}::serialize(node, {0}, \"{0}\");\n", fieldName, format);
    }
    if (t == SerializationType::DESERIALIZATION && model->is_simple_type(fieldType) && (!fieldValue.empty() || fieldType == Objects::STRING.type)) {
        return format_indexes(
            "serializer_{4}::deserialize(node, {0}, \"{0}\", {3}{1}({2}));\n", fieldName,
            fieldType, fieldValue,
            std::string(fieldType == Objects::STRING.type ? "std::" : ""), format);
    }
    if (t == SerializationType::DESERIALIZATION) {
        return format_indexes("serializer_{1}::deserialize(node, {0}, \"{0}\");\n", fieldName, format);
    }
    assert(0);
    return "";
}
