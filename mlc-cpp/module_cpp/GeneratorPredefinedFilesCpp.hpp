//
//  GeneratorPredefinedFilesCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <string>
#include <vector>
#include <utility>

class Model;

class GeneratorPredefinedFiles {
public:
    // Возвращает namespace для файлов
    static std::string getNamespace();

    // Добавляет предопределённые файлы в модель
    void generate(Model &model);

private:
    void generateConfigFiles(Model &model);
    void generateBaseEnumClass(Model &model);
    void generateObserverClass(Model &model);
};

