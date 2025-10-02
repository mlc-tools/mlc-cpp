//
//  GeneratorObserverPython.hpp
//

#pragma once

#include <memory>
#include <string>

class Model;
class Class;

class GeneratorObserverPython {
public:
    void generate(Model &model);
    static std::string getObservableName();
};
