//
//  tests_models.cpp
//  mlc
//
//  Created by Vladimir Tolmachev on 29.11.2025.
//

#include "models/Class.hpp"
#include "core/Parser.hpp"
#include <cassert>

namespace tests{

void test_class_has_virtual(){
    Class cls;
    cls.functions.push_back(std::move(parse_function("fn string get_icon_small():virtual")));
    assert(cls.has_virtual());
}

void run_models_tests(){
    test_class_has_virtual();
}
}
