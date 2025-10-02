//
//  Generator.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorCpp.hpp"
#include "../models/Model.hpp"
#include "GeneratorObserverCpp.hpp"
#include "GeneratorPredefinedFilesCpp.hpp"

void GeneratorCpp::generate(Model &model) {
    GeneratorPredefinedFiles().generate(model);
    GeneratorObserverCpp().generate(model);
}
