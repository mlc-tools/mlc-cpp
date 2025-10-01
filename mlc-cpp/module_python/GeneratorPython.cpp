//
//  GeneratorPython.cpp
//

#include "GeneratorPython.hpp"

#include "../models/Model.hpp"
#include "../models/Class.hpp"
#include "GeneratorFactoryPython.hpp"
#include "GeneratorPackagePython.hpp"
#include "GeneratorObserverPython.hpp"
#include "GeneratorPredefinedFilesPython.hpp"

#include <filesystem>

void GeneratorPython::generate(Model &model)
{
    generateBaseEnumClass(model);
    GeneratorPredefinedFilesPython().generate(model);
    generateExtras(model);
}

void GeneratorPython::generateBaseEnumClass(Model &model)
{
    auto base_enum = std::make_shared<Class>();
    base_enum->name = "BaseEnum";
    base_enum->type = "class";
    model.add_class(base_enum);
}

void GeneratorPython::addPredefinedFiles(Model &model)
{
    GeneratorPredefinedFilesPython().generate(model);
}

void GeneratorPython::generateExtras(Model &model)
{
    GeneratorFactoryPython().generate(model);
    GeneratorPackagePython().generate(model);
    GeneratorObserverPython().generate(model);
}
