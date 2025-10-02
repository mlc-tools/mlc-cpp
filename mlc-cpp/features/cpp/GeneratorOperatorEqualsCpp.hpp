//
//  GeneratorOperatorEqualsCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <memory>
#include <string>

#include "features/GeneratorOperatorEqualsBase.hpp"

class GeneratorOperatorEqualsCpp : public GeneratorOperatorEqualsBase {
public:
    GeneratorOperatorEqualsCpp() = default;

protected:
    std::string getEqualMethodName() const override;
    std::string getNotEqualMethodName() const override;
    std::string getCompareMethodPattern(const std::shared_ptr<Class> &cls,
                                        const Object &member) const override;
    std::string getNotEqualMethodOperation() const override;

    void addCopyConstructor(const std::shared_ptr<Class> &cls) override;
    void addMoveConstructor(const std::shared_ptr<Class> &cls) override;
    void addCopyOperator(const std::shared_ptr<Class> &cls) override;
};
