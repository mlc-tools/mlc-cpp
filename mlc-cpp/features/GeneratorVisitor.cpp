//
//  GeneratorVisitor.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//
#include "GeneratorVisitor.hpp"
#include "../core/Parser.hpp"
#include "../models/Object.hpp"
#include "Error.hpp"
#include <cassert>
#include <iostream>

#include <algorithm>
#include <cctype>
#include <sstream>

GeneratorVisitor::GeneratorVisitor()
    : _model(nullptr), supportOverride_(false) {}

void GeneratorVisitor::generate(Model &model) {
    _model = &model;
    supportOverride_ = _model->config.language == "cpp";

    // 1) Считаем для каждого класса, к какому базовому visitor-интерфейсу он
    // относится
    for (auto &cls : model.classes) {
        if(_model->is_skip(*cls))
            continue;
        
        if (cls->name == "RewardAcceptor")
            std::cout << "\n";
        std::string baseName = getBaseVisitorName(cls);
        if (baseName == "IVisitorDataReward")
            std::cout << "\n";
        if (baseName.empty())
            continue;
        auto &vec = baseVisitorClasses_[baseName];
        if (vec.empty()) {
            // первое попадание — добавляем сам интерфейс
            auto base_cls = model.get_class(baseName);
            if (base_cls)
                vec.push_back(base_cls);
        }
        vec.push_back(cls);
    }

    // 2) Для каждого интерфейса IVisitor<Base> генерим интерфейс и
    // accept-методы
    for (auto &entry : baseVisitorClasses_) {
        const auto &baseName = entry.first;
        const auto &visitors = entry.second;

        generateAcceptorInterface(baseName, visitors);
        for (auto &visitor : visitors) {
            addAcceptMethod(visitor, baseName);
        }
    }

    // 3) Если не поддерживаем override, то переименовываем методы у наследников
    if (!supportOverride_) {
        for (auto &cls : model.classes) {
            // идём по иерархии суперклассов
            std::string sup = cls->parent_class_name;
            if (!sup.empty()) {
                if (std::find(acceptorsInterfaces_.begin(),
                              acceptorsInterfaces_.end(),
                              sup) != acceptorsInterfaces_.end()) {
                    overrideMethods(cls);
                    break;
                }
            }
        }
    }
}

std::string
GeneratorVisitor::getBaseVisitorName(const std::shared_ptr<Class> &cls) {
    auto supName = cls->parent_class_name;
    if (supName.empty())
        return std::string();
    // уже видели этот интерфейс
    if (baseVisitorClasses_.count(supName)) {
        return supName;
    }
    // если нет такого класса в модели — проверяем, может это IVisitorX
    if (!_model->hasClass(supName)) {
        if (supName.rfind("IVisitor", 0) == 0) {
            return std::string(); // это корректный случай
        }
        Error::exit(Error::UNKNOWN_SUPERCLASS, cls->name, supName);
    }
    auto supCls = _model->get_class(supName);
    if (supCls && supCls->is_visitor) {
        return supName;
    }
    // рекурсивно ищем дальше
    if (supCls) {
        auto res = getBaseVisitorName(supCls);
        return res;
    }
    return std::string();
}

void GeneratorVisitor::generateAcceptorInterface(const std::string &baseName, const std::vector<std::shared_ptr<Class>> &visitors) {
    assert(!visitors.empty());
    // создаём новый Class — интерфейс
    auto acceptor = std::make_shared<Class>();
    acceptor->name = "IVisitor" + baseName;
    acceptor->group = visitors.front()->group;
    acceptor->type = "class";
    acceptor->is_abstract = true;
    acceptor->is_virtual = true;
    acceptor->side = visitors.front()->side;

    _model->add_class(acceptor);
    acceptorsInterfaces_.push_back(acceptor->name);

    // метод visit_X для каждого посетителя
    for (auto &visitor : visitors) {
        Function m;
        if (supportOverride_) {
            m.name = "visit";
        } else {
            // visit_lowerFirst(ClassName)
            std::string vn = visitor->name;
            vn[0] = (char)std::tolower(vn[0]);
            m.name = "visit_" + vn;
        }
        m.return_type = Objects::VOID;
        m.callable_args.emplace_back(Object(visitor->name, "ctx"));
        m.callable_args.back().is_pointer = true;
        m.is_abstract = true;
        // запрещаем intrusive для ctx
        m.callable_args.back().denied_intrusive = true;

        acceptor->functions.push_back(std::move(m));
    }

    // если нет override-методов, добавляем общий visit(Base*) с if/else
    if (!supportOverride_) {
        Function m;
        m.name = "visit";
        m.return_type = Objects::VOID;
        m.callable_args.emplace_back(Object(baseName, "ctx"));
        m.callable_args.back().is_pointer = true;
        m.body += "\nif (!ctx) \n{\nreturn;\n }\n";
        for (auto &visitor : visitors) {
            std::string vn = visitor->name;
            vn[0] = (char)std::tolower(vn[0]);
            m.body += format_indexes(
                R"(else if (ctx->get_type() == {0}::TYPE) 
{
    this->visit_{1}(ctx);
}
)",
                visitor->name, vn);
        }
        acceptor->functions.push_back(std::move(m));
    }

    // сортируем методы по типу первого аргумента
    std::sort(acceptor->functions.begin(), acceptor->functions.end(),
              [](auto &a, auto &b) {
                  return a.callable_args[0].type < b.callable_args[0].type;
              });
}

void GeneratorVisitor::addAcceptMethod(const std::shared_ptr<Class> &cls,
                                       const std::string &baseName) {
    Function m;
    m.name = "accept";
    m.return_type = Objects::VOID;
    m.callable_args.emplace_back(Object("IVisitor" + baseName, "visitor"));
    m.callable_args.back().is_pointer = true;
    m.is_virtual = true;
    m.body += "visitor->visit(this);";
    cls->functions.push_back(std::move(m));
}

void GeneratorVisitor::overrideMethods(const std::shared_ptr<Class> &cls) {
    for (auto &m : cls->functions) {
        if (m.name == "visit" && m.callable_args.size() == 1) {
            std::string t = m.callable_args[0].type;
            t[0] = (char)std::tolower(t[0]);
            m.name = "visit_" + t;
        }
    }
}
