//
//  GeneratorRefCounterCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

class Model;
class Class;

class GeneratorRefCounterCpp {
public:
    GeneratorRefCounterCpp();

    // Добавляет счётчик ссылок, retain/release в немодельные классы
    void generate(Model &model);

private:
    std::string preferType_;
    std::string retainBody_;
    std::string releaseBody_;

    // Обработка одного класса
    void addToClass(const std::shared_ptr<Class> &cls);
};
