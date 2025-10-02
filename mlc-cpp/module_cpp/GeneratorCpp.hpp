//
//  Generator.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include "../core/Generator.hpp"

class GeneratorCpp : public Generator {
public:
    virtual void generate(Model &model) override;
};
