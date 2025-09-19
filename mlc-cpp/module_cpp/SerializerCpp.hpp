//
//  SerializerCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//

#pragma once

#include "SerializerBase.hpp"
#include "Object.hpp"
#include "protocols.hpp"   // declares: extern ProtocolMap CPP_XML, CPP_JSON

class SerializerCpp : public SerializerBase {
public:
    SerializerCpp();
    virtual ~SerializerCpp();

protected:
    /// Load the correct protocol map (XML vs JSON).
    ProtocolMap getProtocolMap(const std::string &format) const;

    std::string getParentSerializeCall() const override {
        return "{0}::{1}({2});\n";
    }
    virtual std::string getProtocolText(const std::string &format) override;
    Object getSerializationFunctionArg(SerializationType t, const std::string &format) const override;

    virtual std::string buildSerializeOperation(const std::string &fieldName,
                                        const std::string &fieldType,
                                        const std::string &fieldValue,
                                        SerializationType t,
                                        const std::vector<Object> &tplArgs,
                                        bool isPointer,
                                        bool isLink,
                                        const std::string &format) override;

    virtual std::string buildMapSerialization(const std::string &fieldName,
                                      const std::vector<Object> &tplArgs,
                                      SerializationType t,
                                      const std::string &format) override;

    virtual std::string finalizeSerializeOperation(std::string s) const override {
        return s;
    }

    virtual std::string convertInitializeValue(const std::string &value) const override {
        return value;
    }

private:
    /// Helper for JSON vs XML protocol.
    ProtocolMap protocol;

    /// Build the JSON/XML-specific operation for enums.
    std::string buildEnumOp(const std::string &fieldName,
                            SerializationType t) const;

    /// Build deserialization for map<key,value>.
    std::string buildMapDeserialization(const std::string &fieldName,
                                        const std::vector<std::shared_ptr<Object>> &tplArgs,
                                        const std::string &format);

    std::string ns = "mg";
};
