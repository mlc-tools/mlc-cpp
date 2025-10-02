//
//  Generator.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

class Model;

class Generator {
public:
    virtual void generate(Model &model) = 0;
};
