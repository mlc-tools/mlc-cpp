//
//  GeneratorPython.hpp
//

#pragma once

#include <memory>
#include <string>

class Model;
class Class;

class GeneratorPython {
public:
    GeneratorPython() = default;
    void generate(Model &model);

private:
    void generateBaseEnumClass(Model &model);
    void addPredefinedFiles(Model &model);
    void generateExtras(Model &model);
};
