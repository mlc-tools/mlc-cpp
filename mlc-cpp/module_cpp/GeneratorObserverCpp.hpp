//
//  GeneratorObserverCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//

#pragma once

#include <memory>

class Model;
class Class;

class GeneratorObserverCpp{
public:
    GeneratorObserverCpp() = default;
    void generate(Model& model);
protected:
    void generate_file(Model& model);
    std::shared_ptr<Class> generate_class();
};
