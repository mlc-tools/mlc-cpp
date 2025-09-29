//
//  Mlc.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <cstdlib>
#include <filesystem>

#include "Model.hpp"
#include "Parser.hpp"
#include "Linker.hpp"

class Mlc {
public:
    explicit Mlc(bool use_colors = false, bool disable_logs = false);

    // Основные этапы
    void generate();
    void generateData();
    void runTest();
    
    Model& get_model() { return _model; }

    // Непрерывный режим: следит за изменениями в конфигах и данных,
    // автоматически вызывает generate()/generateData(). Блокирует поток.
    // Параметры:
    //  - poll_ms: период опроса ФС (мс)
    //  - debounce_ms: дебаунс перед регенерацией (мс)
    void watchAndServe(unsigned poll_ms = 300, unsigned debounce_ms = 200);

    // Инкрементальная пересборка: перечитать изменённые .mlc,
    // удалить классы из удалённых .mlc и сгенерировать только затронутые классы
    void generateIncremental(const std::vector<std::string>& changedFiles,
                             const std::vector<std::string>& removedFiles);

private:
    Model _model;
    void buildFeatureGenerators();
};
