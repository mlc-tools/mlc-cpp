//
//  Generator.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorCpp.hpp"
#include "../models/Model.hpp"
#include "GeneratorUnitTestsInterface.hpp"
#include "GeneratorDataStorageCpp.hpp"
#include "GeneratorPredefinedFilesCpp.hpp"
#include "GeneratorVisitor.hpp"
#include "GeneratorRefCounterCpp.hpp"
#include "GeneratorOperatorEqualsCpp.hpp"
#include "GeneratorObserverCpp.hpp"

void GeneratorCpp::generate(Model& model){
    GeneratorUnitTestsInterface().generate(model);
    GeneratorDataStorageCpp().generate(model);
    GeneratorPredefinedFiles().generate(model);
    GeneratorVisitor().generate(model, true);
    GeneratorObserverCpp().generate(model);
    GeneratorRefCounterCpp().generate(model);
    GeneratorOperatorEqualsCpp().generate(model);
}
