//
//  GeneratorOperatorEqualsCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorOperatorEqualsCpp.hpp"
//#include "AccessSpecifier.hpp"
//#include "Error.hpp"
#include "Class.hpp"
#include "Object.hpp"
#include "Function.hpp"
#include "Model.hpp"

std::string GeneratorOperatorEqualsCpp::getEqualMethodName() const {
    return "operator ==";
}

std::string GeneratorOperatorEqualsCpp::getNotEqualMethodName() const {
    return "operator !=";
}

std::string GeneratorOperatorEqualsCpp::getCompareMethodPattern(
    const std::shared_ptr<Class>& /*cls*/,
    const Object& member) const
{
    const std::string base =
        "\nresult = result && this->{0} == rhs.{0};";
    const std::string ptr =
        "\nresult = result && ((this->{0} == rhs.{0}) || (this->{0} != nullptr && rhs.{0} != nullptr && *this->{0} == *rhs.{0}));";

    return (member.is_pointer || member.is_link) ? ptr : base;
}

std::string GeneratorOperatorEqualsCpp::getNotEqualMethodOperation() const {
    return "\nreturn !(*this == rhs);";
}

void GeneratorOperatorEqualsCpp::addCopyConstructor(
    const std::shared_ptr<Class>& cls)
{
    Function ctor;
    ctor.name       = cls->name;
    ctor.return_type = Objects::VOID;
    // const Class& rhs
    ctor.callable_args.push_back(getConstRef(cls, "rhs"));
    ctor.body += "\nthis->operator=(rhs);";
    cls->functions.push_back(std::move(ctor));
}

void GeneratorOperatorEqualsCpp::addMoveConstructor(
    const std::shared_ptr<Class>& /*cls*/)
{
    // no move-constructor by default
}

void GeneratorOperatorEqualsCpp::addCopyOperator(
    const std::shared_ptr<Class>& cls)
{
    Function op;
    op.name       = "operator =";
    op.return_type = getConstRef(cls, "");
    op.callable_args.emplace_back(getConstRef(cls, "rhs"));

    // call parent operator= if needed
    if (!cls->parent_class_name.empty() && cls->parent_class_name != "SerializedObject") {
        op.body += "\nthis->" + cls->parent_class_name + "::operator=(rhs);";
    }

    // assign each member
    for (auto& m : cls->members) {
        if (m.is_static)
            continue;
        if(_model->is_skip(m))
            continue;
        if (_model->side == Side::server && m.name == "_reference_counter") {
            op.body += "\nthis->" + m.name + ".store(rhs." + m.name + ".load());";
        } else {
            op.body += "\nthis->" + m.name + " = rhs." + m.name + ";";
        }
    }

    op.body += "\nreturn *this;";
    cls->functions.push_back(std::move(op));
}

