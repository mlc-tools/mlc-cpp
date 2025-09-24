//
//  SerializerBase.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class Model;
class Class;
class Function;
class Object;

enum SerializationType {
    SERIALIZATION   = 0,
    DESERIALIZATION = 1
};

class SerializerBase {
public:
    SerializerBase();
    virtual ~SerializerBase();
    void generateMethods(Model &model);

protected:
    Model* model;
    Class* currentClass;
    using ProtocolMap = std::vector<std::unordered_map<std::string, std::vector<std::string>>>;
    ProtocolMap serialize_protocol;
    void createSerializationFunction(Class &cls,
                                     SerializationType t,
                                     const std::string &format);
    virtual std::string getProtocolText(const std::string &format) = 0;
    virtual std::string getParentSerializeCall() const = 0;
    virtual Object getSerializationFunctionArg(SerializationType t, const std::string &format) const = 0;
    virtual std::string buildSerializeOperation(const std::string &fieldName,
                                                const std::string &fieldType,
                                                const std::string &fieldValue,
                                                SerializationType t,
                                                const std::vector<Object> &templateArgs,
                                                bool isPointer,
                                                bool isLink,
                                                const std::string &format) = 0;
    virtual std::string buildMapSerialization(const std::string &fieldName,
                                              const std::vector<Object> &templateArgs,
                                              SerializationType t,
                                              const std::string &format) = 0;
    virtual std::string finalizeSerializeOperation(std::string s) const = 0;
    virtual std::string convertInitializeValue(const std::string &value) const = 0;
    std::string dispatchSerializeOp(const std::string &fieldName,
                                    const std::string &fieldType,
                                    const std::string &fieldValue,
                                    SerializationType t,
                                    const std::vector<Object> &templateArgs,
                                    bool isPointer,
                                    bool isLink,
                                    const std::string &format);
    void loadDefaultserialize_protocol(const std::string& formatName);
};
