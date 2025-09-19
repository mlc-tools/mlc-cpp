//
//  GeneratorOperatorEqualsBase.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

class Model;
class Class;
class Object;

class GeneratorOperatorEqualsBase {
public:
    GeneratorOperatorEqualsBase() = default;
    virtual ~GeneratorOperatorEqualsBase() = default;

    // Основной метод: генерирует операторы и конструкторы для всех классов модели
    void generate(Model &model);

protected:
    Model *_model{nullptr};

    // Возвращает Object, представляющий `const ClassName&`
    static Object getConstRef(const std::shared_ptr<Class> &cls, const std::string& name);

    // Имя оператора ==, !=
    virtual std::string getEqualMethodName()    const = 0;
    virtual std::string getNotEqualMethodName() const = 0;

    // Шаблон сравнения одного поля: по умолчанию "result = result && this->{0} == rhs.{0};"
    virtual std::string getCompareMethodPattern(
        const std::shared_ptr<Class> &cls,
        const Object &member) const;

    // Операция оператора != по умолчанию "return !(*this == rhs);"
    virtual std::string getNotEqualMethodOperation() const;

    // Вызов суперкласса для оператора==: по умолчанию "this->{0}::{1}(rhs);"
    virtual std::string getCallSuperclassEqual() const;

    // Методы для добавления копирующих/перемещающих конструкторов и оператора=
    virtual void addCopyConstructor(const std::shared_ptr<Class> &cls);
    virtual void addMoveConstructor(const std::shared_ptr<Class> &cls);
    virtual void addCopyOperator   (const std::shared_ptr<Class> &cls);

private:
    // Вспомогательная замена {0}, {1}, ... в шаблоне pattern на args[i]
    static std::string formatPattern(
        const std::string &pattern,
        const std::vector<std::string> &args);

    // Генерация отдельных частей
    void addEqualMethod    (const std::shared_ptr<Class> &cls);
    void addNotEqualMethod (const std::shared_ptr<Class> &cls);
};


