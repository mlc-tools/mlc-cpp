//
//  GeneratorOperatorEqualsPython.cpp
//

#include "GeneratorOperatorEqualsPython.hpp"
#include "../models/Model.hpp"
#include "../models/Class.hpp"
#include "../models/Object.hpp"
#include "../models/Function.hpp"

std::string GeneratorOperatorEqualsPython::getCompareMethodPattern(const std::shared_ptr<Class> &cls, const Object &member) const {
    static std::string line = "result = result && this->{0} == rhs.{0}\n";
    static std::string line_ptr = "result = result && ((this->{0} == nullptr && rhs.{0} == nullptr) || (this->{0} != nullptr && rhs.{0} != nullptr && this->{0} == rhs.{0}))\n";
    return member.is_pointer || member.is_link ? line_ptr : line;
}
