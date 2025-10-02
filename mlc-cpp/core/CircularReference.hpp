//
//  CircularReference.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class Model;
class Class;

class CircularReference {
public:
    explicit CircularReference(Model &model);

    // Запускает поиск циклических ссылок
    void find();

private:
    // Рекурсивно обходит граф «baseClass → члены → классы»,
    // собирая имена в circularReference_ при обнаружении цикла
    void wave(const std::shared_ptr<Class> &baseClass,
              const std::shared_ptr<Class> &rootClass);

    Model &_model;
    std::unordered_set<std::string> viewed_;
    std::vector<std::string> circularReference_;
};
