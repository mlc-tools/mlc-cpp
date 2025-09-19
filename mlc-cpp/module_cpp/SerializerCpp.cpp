//
//  SerializerCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#include "SerializerCpp.hpp"
#include "Model.hpp"
#include "Common.hpp"
#include "Error.hpp"    // for Error::exit
#include <cassert>


std::string convert_type(const std::string& type_)
{
    if(type_ == "list") return "std::vector";
    if(type_ == "map") return "std::map";
    if(type_ == "string") return "std::string";
    return type_;
}

SerializerCpp::SerializerCpp()
: SerializerBase()
, protocol() {
}
SerializerCpp::~SerializerCpp(){
    
}

SerializerCpp::ProtocolMap SerializerCpp::getProtocolMap(const std::string &format) const {
//    return (format == "xml" ? CPP_XML : CPP_JSON);
    return {};
}

std::string SerializerCpp::getProtocolText(const std::string &format){
    return "";
}

Object SerializerCpp::getSerializationFunctionArg(SerializationType t, const std::string &format) const
{
    auto makeObj = [&](const std::string &type, const std::string &name){
        Object obj;
        obj.type = type;
        obj.name = name;
        obj.is_ref = true;
        return obj;
    };
    if (format=="json") {
        if (t==SERIALIZATION)
            return makeObj("SerializerJson", "serializer");
        else
            return makeObj("DeserializerJson", "deserializer");
    } else {
        if (t==SERIALIZATION)
            return makeObj("SerializerXml", "serializer");
        else
            return makeObj("DeserializerXml", "deserializer");
    }
}

std::string SerializerCpp::buildSerializeOperation(
    const std::string &fieldName,
    const std::string &fieldType,
    const std::string &fieldValue,
    SerializationType t,
    const std::vector<Object> &tplArgs,
    bool isPointer,
    bool isLink,
    const std::string &format)
{
    // Quick-path for primitives with default
    if (t==SERIALIZATION
        && model->is_simple_type(fieldType)
        && (!fieldValue.empty() || fieldType == Objects::STRING.type))
    {
        return format_indexes(
            "serializer.serialize({0}, \"{0}\", {3}{1}({2}));\n",
            fieldName, fieldType, fieldValue, std::string(fieldType == Objects::STRING.type ? "std::" : ""));
    }
    if (t==SERIALIZATION) {
        return format_indexes("serializer.serialize({0}, \"{0}\");\n", fieldName);
    }
    if (t==DESERIALIZATION
        && model->is_simple_type(fieldType)
        && (!fieldValue.empty() || fieldType == Objects::STRING.type))
    {
        return format_indexes(
            "deserializer.deserialize({0}, \"{0}\", {3}{1}({2}));\n",
            fieldName, fieldType, fieldValue, std::string(fieldType == Objects::STRING.type ? "std::" : ""));
    }
    if (t==DESERIALIZATION) {
        return format_indexes("deserializer.deserialize({0}, \"{0}\");\n", fieldName);
    }
    // Fallback to full protocol dispatch:
    protocol = getProtocolMap(format);
    return dispatchSerializeOp(fieldName,
                               fieldType,
                               fieldValue,
                               t,
                               tplArgs,
                               isPointer,
                               isLink,
                               format);
}

std::string SerializerCpp::buildMapSerialization(
    const std::string &fieldName,
    const std::vector<Object> &tplArgs,
    SerializationType t,
    const std::string &format)
{
    assert(tplArgs.size()==2);
    auto &key   = tplArgs[0];
    auto &value = tplArgs[1];
    // use protocol["map"][0]
    const auto& pat = protocol[t]["map"][0];
    // recursively serialize key/value
    auto ks = buildSerializeOperation("key",   key.type,   "", SERIALIZATION, {}, key.is_pointer, key.is_link, format);
    auto vs = buildSerializeOperation("value", value.type, "", SERIALIZATION, value.template_args, value.is_pointer, value.is_link, format);
    return ::format(pat, {
        {"field",          fieldName},
        {"key_serialize",  ks},
        {"value_serialize",vs}})
         + "\n\n";
}

std::string SerializerCpp::buildMapDeserialization(
    const std::string &fieldName,
    const std::vector<std::shared_ptr<Object>> &tplArgs,
    const std::string &format)
{
    assert(tplArgs.size()==2);
    auto &key   = *tplArgs[0];
    auto &value = *tplArgs[1];
    const auto& pat = protocol[DESERIALIZATION].at("map")[0];
    // declare key
    std::string keyDecl = key.is_pointer
        ? format_indexes("auto key = make_intrusive<{0}>();", key.type)
        : format_indexes("{0} key;", convertInitializeValue(key.type));
    auto ks = buildSerializeOperation("key",   key.type,   "", DESERIALIZATION, {}, key.is_pointer, key.is_link, format);
    auto vs = buildSerializeOperation("value", value.type, "", DESERIALIZATION, value.template_args, value.is_pointer, value.is_link, format);
    std::string valInit = value.is_pointer
        ? format_indexes("intrusive_ptr<{0}>", value.type)
        : convert_type(value.type);
    return ::format(pat, {
        {"field",          fieldName},
        {"key",            keyDecl},
        {"key_serialize",  ks},
        {"value_serialize",vs},
        {"value_type",     valInit}})
         + "\n\n";
}
