//
//  GeneratorRefCounterCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "features/FeatureGenerator.hpp"

class Model;
class Class;

class GeneratorRefCounterCpp : public FeatureGenerator {
public:
    GeneratorRefCounterCpp();

    virtual void generate(Model &model) override;
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) override{}

private:
    std::string preferType_;
    std::string retainBody_;
    std::string releaseBody_;

    void addToClass(const std::shared_ptr<Class> &cls);
};

