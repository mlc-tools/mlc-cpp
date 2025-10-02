//
//  WriterBase.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//
#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class Model;
class Class;
class Function;
class Object;
enum class SerializeFormat;

class WriterBase {
public:
    explicit WriterBase();
    virtual ~WriterBase() = default;

    // Сохраняет все auto_generated классы через writeClass
    void save(Model &model);
    void set_model(Model &model) { _model = &model; }

protected:
    Model *_model;
    void *serializer_; // можно заменить на конкретный тип сериализатора
    std::string outDirectory_;
    // Пары (Class*, (filename, content)) будут переданы модели
    std::vector<
        std::pair<std::shared_ptr<Class>, std::pair<std::string, std::string>>>
        files_;
    std::vector<std::string> createdFiles_;
    std::shared_ptr<Class> currentClass_;

    // Перегружаемый метод: должен вернуть вектор (filename, content)
    virtual std::vector<std::pair<std::string, std::string>>
    writeClass(const std::shared_ptr<Class> &cls);

    // Устанавливает initial_value для enum'а
    void setInitialValues(const std::shared_ptr<Class> &cls);

    // Возвращает (constructorArgs, constructorBody)
    std::pair<std::string, std::string>
    getConstructorData(const std::shared_ptr<Class> &cls);

    // Функции
    virtual std::pair<std::string, std::string>
    writeFunction(const Function &method);
    std::string createFunctionArgsString(const Function &method);
    std::string getArgumentTypeName(const Object &arg) const;
    std::string getMethodArgPattern(const Object &arg) const;
    std::string getMethodPattern(const Function &method) const;
    virtual std::string getRequiredArgs(const Function &method) const;
    virtual std::string getNullptrString() const;
    std::string addStaticModifierToMethod(const std::string &text) const;

    // Объекты
    virtual std::tuple<std::string, std::string, std::string>
    writeObject(const Object &obj);

    virtual std::string convertInitializeValue(const std::string &value) {
        return value;
    }

    // Подготовка текста файла: убираем блоки форматов xml/json
public:
    std::string prepareFile(const std::string &text) const;
    std::string prepareFileCodeStylePhp(const std::string &text) const;
    // Indentation similar to original Python writer (4 spaces per tab),
    // with handling of 'public:/protected:/private:' blocks and braces.
    std::string prepareFileCodeStyleCpp(const std::string &text) const;
};
