//
//  GeneratorOperatorEqualsCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorOperatorEqualsCpp.hpp"
// #include "AccessSpecifier.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Model.hpp"
#include "../models/Object.hpp"
#include "../core/Parser.hpp"
#include "Error.hpp"
#include <cassert>
#include <iostream>

void GeneratorOperatorEqualsCpp::generate(Model &model) {
    for (auto &cls : model.classes) {
        GeneratorOperatorEqualsCpp::generate_signaturs(cls);
    }
    GeneratorOperatorEqualsBase::generate(model);
}

void GeneratorOperatorEqualsCpp::generate_signaturs(const std::shared_ptr<Class> &cls){
    if (cls->type == "enum")
        return;
    if (cls->is_inline)
        return;
    if (cls->name == "DataStorage")
        return;

    if(!cls->is_discard_copy_ctr() && !cls->get_copy_constructor()){
        cls->functions.push_back(parse_function(format_indexes("fn void {0}({0}:ref:const rhs)", cls->name)));
        assert(cls->get_copy_constructor());
    }
    if(!cls->is_discard_move() && !cls->get_move_constructor()){
        auto func = parse_function(format_indexes("fn void {0}({0}&& rhs)", cls->name));
        func.is_noexcept = true;
        cls->functions.push_back(std::move(func));
        assert(cls->get_move_constructor());
    }
    if(!cls->is_discard_copy() && !cls->get_copy_operator()){
        auto func = parse_function(format_indexes("fn {0}:ref operator({0}:ref:const rhs)", cls->name));
        func.name = "operator =";
        cls->functions.push_back(std::move(func));
        assert(cls->get_copy_operator());
    }
    if(!cls->is_discard_move() && !cls->get_move_operator()){
        auto func = parse_function(format_indexes("fn {0}:ref operator({0}&& rhs)", cls->name));
        func.name = "operator =";
        func.is_noexcept = true;
        cls->functions.push_back(std::move(func));
        assert(cls->get_move_operator());
    }
}

std::string GeneratorOperatorEqualsCpp::getEqualMethodName() const {
    return "operator ==";
}

std::string GeneratorOperatorEqualsCpp::getNotEqualMethodName() const {
    return "operator !=";
}

std::string GeneratorOperatorEqualsCpp::getCompareMethodPattern(const std::shared_ptr<Class> & /*cls*/, const Object &member) const {
    static const std::string base = "\nresult = result && this->{0} == rhs.{0};";
    static const std::string ptr =
        "\nresult = result && ((this->{0} == rhs.{0}) || (this->{0} != nullptr "
        "&& rhs.{0} != nullptr && *this->{0} == *rhs.{0}));";

    return (member.is_pointer || member.is_link) ? ptr : base;
}

std::string GeneratorOperatorEqualsCpp::getNotEqualMethodOperation() const {
    return "\nreturn !(*this == rhs);";
}

void GeneratorOperatorEqualsCpp::addCopyConstructor(const std::shared_ptr<Class> &cls) {
    auto ctor = cls->get_copy_constructor();
    if(!ctor)
        return;
    if(cls->has_member("_reference_counter")){
        ctor->body += "\nthis->_reference_counter = 1;";;
    }
    ctor->body += "\nthis->operator = (rhs);";
}


void GeneratorOperatorEqualsCpp::addMoveConstructor(const std::shared_ptr<Class> &cls) {
    auto ctor = cls->get_move_constructor();
    if(!ctor)
        return;

    if (!cls->parent_class_name.empty() && cls->parent_class_name != "SerializedObject"){
        ctor->ctor_initializations += "\n: " + cls->parent_class_name + "(std::move(rhs))";
    }

    std::string delimiter = ctor->ctor_initializations.empty() ? ": " : ", ";
    for (auto &m : cls->members) {
        if (m.is_static || m.is_const)
            continue;
        if (_model->is_skip(m))
            continue;
        if(m.name == "_reference_counter")
            ctor->ctor_initializations += "\n" + delimiter + m.name + "(1)";
        else
            ctor->ctor_initializations += "\n" + delimiter + m.name + "(std::move(rhs." + m.name + "))";
        delimiter = ", ";
    }
}

void GeneratorOperatorEqualsCpp::addMoveOperator(const std::shared_ptr<Class> &cls) {
    auto op = cls->get_move_operator();
    if(!op)
        return;

    if (!cls->parent_class_name.empty() &&
        cls->parent_class_name != "SerializedObject")
        op->body += "\nthis->" + cls->parent_class_name + "::operator = (std::move(rhs));";

    for (auto &m : cls->members) {
        if (m.is_static || m.is_const)
            continue;
        if (_model->is_skip(m))
            continue;
        if(m.name == "_reference_counter")
            continue;
        op->body += "\nthis->" + m.name + " = std::move(rhs." + m.name + ");";
    }
    op->body += "\nreturn *this;";
}

void GeneratorOperatorEqualsCpp::addCopyOperator(const std::shared_ptr<Class> &cls) {
    auto op = cls->get_copy_operator();
    if(!op)
        return;

    if (!cls->parent_class_name.empty() &&
        cls->parent_class_name != "SerializedObject")
        op->body += "\nthis->" + cls->parent_class_name + "::operator = (rhs);";

    for (auto &m : cls->members) {
        if (m.is_static || m.is_const)
            continue;
        if (_model->is_skip(m))
            continue;
        if (m.name == "_reference_counter")
            continue;
        else
            op->body += "\nthis->" + m.name + " = rhs." + m.name + ";";
    }

    op->body += "\nreturn *this;";
}
