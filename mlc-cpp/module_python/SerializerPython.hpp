//
//  SerializerPython.hpp
//

#pragma once

#include "../core/SerializerBase.hpp"
#include <utility>
#include <vector>

class SerializerPython : public SerializerBase {
public:
    SerializerPython();
    ~SerializerPython();

public:
    std::string getProtocolText(const std::string &format) override;
    virtual void
    createSerializationFunction(Class &cls, SerializationType t,
                                const std::string &format) override;
    std::string getParentSerializeCall() const override;
    Object
    getSerializationFunctionArg(SerializationType t,
                                const std::string &format) const override;
    std::string buildSerializeOperation(const std::string &fieldName,
                                        const std::string &fieldType,
                                        const std::string &fieldValue,
                                        SerializationType t,
                                        const std::vector<Object> &tplArgs,
                                        bool isPointer, bool isLink,
                                        const std::string &format) override;
    std::string finalizeSerializeOperation(std::string s) const override;
    std::string convertInitializeValue(const std::string &value) const override;

    std::pair<std::string, std::vector<std::string>>
    create_meta_class(const Object &obj) const;
};
