//
//  Config.hpp
//  mlc-cpp
//
//  Configuration for generation and config file loader
//
#pragma once

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "../models/Object.hpp"    // Side
#include "../models/Serialize.hpp" // SerializeFormat

class Job {
public:
    // Параметры задачи генерации
    std::string out_directory;
    std::string out_data_directory;
    std::string language = "cpp";
    std::string namespace_name = "mg";
    Side side = Side::both;
    bool only_data = false;
    bool php_validate = false;
    bool allow_different_virtual = true;
    std::string test_script;
    std::string test_script_args;
    bool generate_tests = false;
    bool generate_intrusive = true;
    bool generate_factory = true;
    bool join_to_one_file = false;
    bool auto_registration = true;
    bool generate_ref_counter = true;
    bool user_includes = false;
    bool empty_methods = false;
    int serializeFormats = static_cast<int>(SerializeFormat::Xml) |
                           static_cast<int>(SerializeFormat::Json);
    std::vector<SerializeFormat> serialize_protocol;
    std::function<bool(const std::string &)> filter_code;
    std::function<bool(const std::string &)> filter_data;
};

class FeatureVisitor {
public:
};
class FeatureUnitTests {
public:
};

class FeatureDataStorage {
public:
    bool private_members = true;
};

class FeatureRefCounter {
public:
};

class FeatureOperatorEquals {
public:
};
class FeatureEcs {
public:
    std::string model_base;
    std::string component_base;
};

class Config {
public:
    operator bool() const { return _initialized; }
    void initialize() { _initialized = true; }
    // Источники (глобальные для конфиг-файла, но дублируются в каждую задачу
    // при загрузке)
    std::vector<std::string> configs_directories;
    std::vector<std::string> data_directories;
    std::vector<Job> jobs;
    std::vector<std::variant<FeatureVisitor, FeatureEcs, FeatureUnitTests,
                             FeatureDataStorage, FeatureRefCounter,
                             FeatureOperatorEquals>>
        features;

    // Загрузка конфигурации из JSON-файла. Возвращает набор задач.
    static Config loadFile(const std::string &path, std::string &err);
    static Config loadString(const std::string &content, std::string &err);

private:
    bool _initialized = false;
};

std::function<bool(const std::string &)>
make_re2_filter_from_patterns(const std::vector<std::string> &raws);
std::vector<std::string>
split_comma_allow_lists(const std::vector<std::string> &vals);
