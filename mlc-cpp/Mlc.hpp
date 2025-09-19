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

class CustomGenerator;  // вперед объявление, если есть

class Mlc {
public:
    explicit Mlc(bool use_colors = false, bool disable_logs = false);

    // Настройка путей и флагов
    void addConfigsDirectory(const std::string &dir);
    void setOutDirectory(const std::string &dir);
    void addDataDirectory(const std::string &dir);
    void setOutDataDirectory(const std::string &dir);
    void setLanguage(const std::string &lang);
    void setOnlyData(bool only);
    void setNamespace(const std::string &ns);
    void setSide(const Side &side);
    void setPhpValidate(bool v);
    void setValidateAllowDifferentVirtualMethod(bool v);
    void setTestScript(const std::string &script, const std::string &args = "");
    void setGenerateTests(bool gen);
    void setGenerateIntrusive(bool gen);
    void setGenerateFactory(bool gen);
    void setJoinToOneFile(bool j);
    void setAutoRegistration(bool ar);
    void setGenerateRefCounter(bool grc);
    void setUserIncludes(bool ui);
    void setEmptyMethods(bool em);
    void setSerializeFormats(int formats);

    // Фильтры для .mlc-файлов и данных
    void setFilterCode(std::function<bool(const std::string &path)> f);
    void setFilterData(std::function<bool(const std::string &path)> f);

    // Основные этапы
    void generate();
    void generateData();
    void runTest();

    // Пользовательский генератор
    void setUserGenerator(std::shared_ptr<CustomGenerator> gen);
    
    Model& get_model() { return _model; }

private:
    Model _model;
    std::function<bool(const std::string &)> _filter_code;
    std::function<bool(const std::string &)> _filter_data;
    std::shared_ptr<CustomGenerator> _custom_generator;

    void runUserGeneratorInternal();
};
