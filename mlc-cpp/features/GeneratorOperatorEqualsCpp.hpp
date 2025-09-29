//
//  GeneratorOperatorEqualsCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <memory>
#include <string>

#include "FeatureGenerator.hpp"
#include "../core/GeneratorOperatorEqualsBase.hpp"

class GeneratorOperatorEqualsCpp : public FeatureGenerator, public GeneratorOperatorEqualsBase {
public:
    GeneratorOperatorEqualsCpp() = default;
    
    virtual void generate(Model &model) override { GeneratorOperatorEqualsBase::generate(model); }
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) override{}

protected:
    std::string getEqualMethodName() const override;
    std::string getNotEqualMethodName() const override;
    std::string getCompareMethodPattern(
        const std::shared_ptr<Class>& cls,
        const Object& member) const override;
    std::string getNotEqualMethodOperation() const override;

    void addCopyConstructor(const std::shared_ptr<Class>& cls) override;
    void addMoveConstructor(const std::shared_ptr<Class>& cls) override;
    void addCopyOperator(const std::shared_ptr<Class>& cls) override;
};

