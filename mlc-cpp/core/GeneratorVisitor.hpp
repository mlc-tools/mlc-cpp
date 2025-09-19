//
//  GeneratorVisitor.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//
#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "Model.hpp"
#include "Class.hpp"
#include "Function.hpp"

class GeneratorVisitor {
public:
    GeneratorVisitor();

    // Основной метод: model — ваша модель, supportOverride — флаг override-методов
    void generate(Model &model, bool supportOverride);

    // Добавить метод accept в класс cls
    static void addAcceptMethod(const std::shared_ptr<Class>& cls,
                                const std::string& baseVisitorName);

    // Переименовать visit(ctx) → visit_Type(ctx)
    static void overrideMethods(const std::shared_ptr<Class>& cls);

private:
    // Ищет первую в иерархии super-классов точку входа visitor-интерфейса
    std::string getBaseVisitorName(const std::shared_ptr<Class>& cls);

    // Генерирует interface IVisitor<Base> и его методы visit_X
    void generateAcceptorInterface(const std::string& baseVisitorName,
                                   const std::vector<std::shared_ptr<Class>>& visitors);

    Model* _model;
    bool supportOverride_;
    // ключ = имя базового visitor-интерфейса, значение = [базовый интерфейс, все наследники]
    std::map<std::string, std::vector<std::shared_ptr<Class>>> baseVisitorClasses_;
    // список сгенерированных acceptor-интерфейсов
    std::vector<std::string> acceptorsInterfaces_;
};
