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

class custom_generator;  // вперед объявление, если есть

class Mlc {
public:
    explicit Mlc(bool use_colors = false, bool disable_logs = false);

    // Настройка путей и флагов
    void addConfigsDirectory(const std::string &dir);
    void setOutDirectory(const std::string &dir);
    void addDataDirectory(const std::string &dir);
    void setOutDataDirectory(const std::string &dir);
    void setLanguage(const std::string &lang);
    void setonly_data(bool only);
    void setNamespace(const std::string &ns);
    void setSide(const Side &side);
    void setphp_validate(bool v);
    void setValidateallow_different_virtualMethod(bool v);
    void settest_script(const std::string &script, const std::string &args = "");
    void setgenerate_tests(bool gen);
    void setgenerate_intrusive(bool gen);
    void setgenerate_factory(bool gen);
    void setjoin_to_one_file(bool j);
    void setauto_registration(bool ar);
    void setgenerate_ref_counter(bool grc);
    void setuser_includes(bool ui);
    void setempty_methods(bool em);
    void setSerializeFormats(int formats);

    // Фильтры для .mlc-файлов и данных
    void setfilter_code(std::function<bool(const std::string &path)> f);
    void setfilter_data(std::function<bool(const std::string &path)> f);

    // Основные этапы
    void generate();
    void generateData();
    void runTest();

    // Пользовательский генератор
    void setUserGenerator(std::shared_ptr<custom_generator> gen);
    
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
    std::function<bool(const std::string &)> _filter_code;
    std::function<bool(const std::string &)> _filter_data;
    std::shared_ptr<custom_generator> _custom_generator;

    void runUserGeneratorInternal();
};
