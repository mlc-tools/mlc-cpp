//
//  GeneratorOperatorEqualsPython.hpp
//

#pragma once

#include "features/GeneratorOperatorEqualsBase.hpp"
#include <memory>

class Model;
class Class;

class GeneratorOperatorEqualsPython : public GeneratorOperatorEqualsBase {
public:
private:
    virtual std::string getEqualMethodName() const override { return "__eq__"; }
    virtual std::string getNotEqualMethodName() const override {
        return "__ne__";
    }
    virtual std::string
    getCompareMethodPattern(const std::shared_ptr<Class> &cls,
                            const Object &member) const override;

    // Операция оператора != по умолчанию "return !(*this == rhs);"
    virtual std::string getNotEqualMethodOperation() const override {
        return "return !(this == rhs)\n";
    }

    // Вызов суперкласса для оператора==: по умолчанию "this->{0}::{1}(rhs);"
    virtual std::string getCallSuperclassEqual() const override {
        return "{0}.{1}(self, rhs)\n";
    }
};
