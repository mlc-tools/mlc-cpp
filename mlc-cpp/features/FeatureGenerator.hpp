//
//  FeatureGenerator.hpp
//  mlc-cpp
//

#pragma once

#include <memory>
#include <string>

class Model;
class Class;

class FeatureGenerator{
public:
    virtual ~FeatureGenerator() = default;
    virtual void generate(Model& model) = 0;
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) = 0;
};

