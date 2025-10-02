//
//  SerializerCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//

#pragma once

#include "Object.hpp"
#include "SerializerBase.hpp"

class SerializerCpp : public SerializerBase {
public:
    SerializerCpp();
    virtual ~SerializerCpp();

protected:
    std::string getParentSerializeCall() const override {
        return "{0}::{1}({2});\n";
    }
    virtual std::string getProtocolText(const std::string &format) override;
    Object
    getSerializationFunctionArg(SerializationType t,
                                const std::string &format) const override;

    virtual std::string
    buildSerializeOperation(const std::string &fieldName,
                            const std::string &fieldType,
                            const std::string &fieldValue, SerializationType t,
                            const std::vector<Object> &tplArgs, bool isPointer,
                            bool isLink, const std::string &format) override;

    virtual std::string
    finalizeSerializeOperation(std::string s) const override {
        return s;
    }

    virtual std::string
    convertInitializeValue(const std::string &value) const override {
        return value;
    }

private:
    std::string buildEnumOp(const std::string &fieldName,
                            SerializationType t) const;

    std::string ns = "mg";
};
