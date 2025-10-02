//
//  GeneratorOperatorEqualsBase.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorOperatorEqualsBase.hpp"
#include "Class.hpp"
#include "Function.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include <algorithm>

void GeneratorOperatorEqualsBase::generate(Model &model) {
    _model = &model;
    for (auto &cls : model.classes) {
        if (cls->type == "enum")
            continue;
        if (cls->is_inline)
            continue;
        if (cls->name == "DataStorage")
            continue;

        addEqualMethod(cls);
        addNotEqualMethod(cls);
        addCopyConstructor(cls);
        addMoveConstructor(cls);
        addCopyOperator(cls);
    }
}

Object
GeneratorOperatorEqualsBase::getConstRef(const std::shared_ptr<Class> &cls,
                                         const std::string &name) {
    Object ref;
    ref.type = cls->name;
    ref.name = name;
    ref.is_const = true;
    ref.is_ref = true;
    return ref;
}

std::string GeneratorOperatorEqualsBase::getCompareMethodPattern(
    const std::shared_ptr<Class> & /*cls*/, const Object & /*member*/) const {
    return "result = result && this->{0} == rhs.{0};";
}

std::string GeneratorOperatorEqualsBase::getNotEqualMethodOperation() const {
    return "return !(*this == rhs);";
}

std::string GeneratorOperatorEqualsBase::getCallSuperclassEqual() const {
    return "this->{0}::{1}(rhs);";
}

void GeneratorOperatorEqualsBase::addEqualMethod(
    const std::shared_ptr<Class> &cls) {
    Function op;
    op.name = getEqualMethodName();
    op.return_type = Objects::BOOL;
    op.callable_args.push_back(getConstRef(cls, "rhs"));
    op.is_const = true;

    // Вызов суперкласса или инициализация результата
    if (cls->parent_class_name.empty()) {
        op.body += ("bool result = true;\n");
    } else {
        auto sup = _model->get_class(cls->parent_class_name);
        if (!sup || sup->is_inline) {
            op.body += ("\nbool result = true;\n");
        } else {
            std::string pattern = getCallSuperclassEqual();
            std::string line =
                formatPattern("\nbool result = " + pattern,
                              {cls->parent_class_name, getEqualMethodName()});
            op.body += (line);
        }
    }

    // Сравнение по полям
    for (const auto &member : cls->members) {
        if (member.is_static || member.is_const ||
            member.type == "Observable" || member.name == "_reference_counter")
            continue;
        bool skip = false;
        for (const auto &t : member.template_args) {
            if (t.type == "Observable") {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;

        if (_model->is_skip(member))
            continue;

        std::string pattern = getCompareMethodPattern(cls, member);
        op.body += formatPattern(pattern, {member.name});
    }

    op.body += "\nreturn result;";
    cls->functions.push_back(std::move(op));
}

void GeneratorOperatorEqualsBase::addNotEqualMethod(
    const std::shared_ptr<Class> &cls) {
    Function op;
    op.name = getNotEqualMethodName();
    op.return_type = Objects::BOOL;
    op.callable_args.emplace_back(getConstRef(cls, "rhs"));
    op.is_const = true;
    op.body += getNotEqualMethodOperation();
    cls->functions.push_back(std::move(op));
}

void GeneratorOperatorEqualsBase::addCopyConstructor(
    const std::shared_ptr<Class> & /*cls*/) {
    // no-op by default
}

void GeneratorOperatorEqualsBase::addMoveConstructor(
    const std::shared_ptr<Class> & /*cls*/) {
    // no-op by default
}

void GeneratorOperatorEqualsBase::addCopyOperator(
    const std::shared_ptr<Class> & /*cls*/) {
    // no-op by default
}

std::string GeneratorOperatorEqualsBase::formatPattern(
    const std::string &pattern, const std::vector<std::string> &args) {
    std::string out = pattern;
    for (size_t i = 0; i < args.size(); ++i) {
        std::string placeholder = "{" + std::to_string(i) + "}";
        size_t pos = 0;
        while ((pos = out.find(placeholder, pos)) != std::string::npos) {
            out.replace(pos, placeholder.size(), args[i]);
            pos += args[i].size();
        }
    }
    return out;
}
