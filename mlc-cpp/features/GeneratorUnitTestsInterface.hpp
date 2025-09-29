//
//  GeneratorUnitTestsInterface.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 29.07.2025.
//
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "FeatureGenerator.hpp"

class Model;
class Class;
class Function;

class GeneratorUnitTestsInterface : public FeatureGenerator {
public:
    GeneratorUnitTestsInterface() = default;

    // Основная точка входа
    virtual void generate(Model &model) override;
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) override{};
private:
    // Генерация «базовых» тестовых классов (TestCase, Logger и т.п.)
    void generateBaseClasses();

    // Создаёт ITest<Class> и возвращает nullptr, если для cls нет наследников
    std::shared_ptr<Class> generateTestInterface(const std::shared_ptr<Class>& cls);

    // Добавляет в class_ метод test_<name>
    void addMethod(std::shared_ptr<Class>& class_, const std::string &name);

    // Создаёт класс RunAllTests
    std::shared_ptr<Class> generateAllTestsClass();

    // Если в методе нет сообщений у assert*, добавляет их по шаблону
    static void generateMessagesIfEmpty(const std::shared_ptr<Class>& cls,
                                        Function &method);

    // Преобразует CamelCase → snake_case
    static std::string getMemberName(const std::string &clsName);

    Model *_model{nullptr};
    std::vector<std::shared_ptr<Class>> tests_;

    // Карта assert-методов → число параметров (из ASSERTS)
    static const std::map<std::string,int> ASSERTS;

    // Текст базовых классов
    static const std::string BASE_CLASSES;
};

