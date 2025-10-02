//
//  GeneratorUnitTestsInterface.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 29.07.2025.
//
#pragma once

#include "FeatureGenerator.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

class Model;
class Class;
class Function;

class GeneratorUnitTestsInterface : public FeatureGenerator {
public:
    GeneratorUnitTestsInterface() = default;

    virtual void generate(Model &model) override;
    virtual void modifySources(Model &model, const std::shared_ptr<Class> &cls,
                               std::string &header,
                               std::string &source) override {};

private:
    void generateBaseClasses();

    std::shared_ptr<Class>
    generateTestInterface(const std::shared_ptr<Class> &cls);

    void addMethod(std::shared_ptr<Class> &class_, const std::string &name);

    std::shared_ptr<Class> generateAllTestsClass();

    static void generateMessagesIfEmpty(const std::shared_ptr<Class> &cls,
                                        Function &method);

    static std::string getMemberName(const std::string &clsName);

    Model *_model{nullptr};
    std::vector<std::shared_ptr<Class>> tests_;

    static const std::map<std::string, int> ASSERTS;

    static const std::string BASE_CLASSES;
};
