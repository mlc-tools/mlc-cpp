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
#include "Error.hpp"

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
    Function ctor;
    ctor.name = cls->name;
    ctor.return_type = Objects::VOID;
    ctor.callable_args.push_back(getConstRef(cls, "rhs"));
    
    if(cls->has_member("_reference_counter")){
        ctor.body += "\nthis->_reference_counter = 1;";;
    }
    ctor.body += "\nthis->operator=(rhs);";
    cls->functions.push_back(std::move(ctor));
}

void GeneratorOperatorEqualsCpp::addMoveConstructor(
    const std::shared_ptr<Class> &cls) {
    Function ctor;
    ctor.name = cls->name;
    ctor.return_type = Objects::VOID;

    Object arg;
    arg.type = cls->name + "&&";
    arg.name = "rhs";
    ctor.callable_args.push_back(std::move(arg));

    if (!cls->parent_class_name.empty() &&
        cls->parent_class_name != "SerializedObject")
        ctor.body += "\nthis->" + cls->parent_class_name + "::operator=(rhs);";

    for (auto &m : cls->members) {
        if (m.is_static || m.is_const)
            continue;
        if (_model->is_skip(m))
            continue;
        if(m.name == "_reference_counter")
            continue;
        ctor.body += "\nthis->" + m.name + " = std::move(rhs." + m.name + ");";
    }

    cls->functions.push_back(std::move(ctor));
}

void GeneratorOperatorEqualsCpp::addCopyOperator(const std::shared_ptr<Class> &cls) {
    Function op;
    op.name = "operator =";
    op.return_type = getConstRef(cls, "");
    op.callable_args.emplace_back(getConstRef(cls, "rhs"));
    
    if (!cls->parent_class_name.empty() &&
        cls->parent_class_name != "SerializedObject")
        op.body += "\nthis->" + cls->parent_class_name + "::operator=(rhs);";

    for (auto &m : cls->members) {
        if (m.is_static || m.is_const)
            continue;
        if (_model->is_skip(m))
            continue;
        if (m.name == "_reference_counter")
            continue;
        else
            op.body += "\nthis->" + m.name + " = rhs." + m.name + ";";
    }

    op.body += "\nreturn *this;";
    cls->functions.push_back(std::move(op));
}
