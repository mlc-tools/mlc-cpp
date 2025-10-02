//
//  GeneratorDataStoragePython.hpp
//

#pragma once

#include "../features/GeneratorDataStorageBase.hpp"

class GeneratorDataStoragePython : public GeneratorDataStorageBase {
public:
    GeneratorDataStoragePython();
    virtual void generate(Model &model) override;

protected:
    bool isNeedCreateStaticInstance() const override { return true; }
    std::string getSharedMethodBody() override;
    std::string getPatternGetter() override;
    std::string getInitializeFunctionJsonBody() override;
    std::string getInitializeFunctionXmlBody() override;

private:
    void addSerializeJson(Model &model);
    void addSerializeXml(Model &model);
    void addSerializeMethod(const std::string &name);
    void addDeserializeJson(Model &model);
    void addDeserializeXml(Model &model);
    void addDeserializeMethod(const std::string &name);
};
