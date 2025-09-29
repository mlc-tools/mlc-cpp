//
//  Model.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <memory>
#include <map>
#include <functional>
#include "Class.hpp"
#include "Object.hpp"
#include "Function.hpp"
#include "Serialize.hpp"
#include "../features/FeatureGenerator.hpp"
#include "../utils/Config.hpp"

class Parser;
class Model;

// FeatureGenerator is declared in features/FeatureGenerator.hpp

class Model {
public:
    using FileEntry = std::tuple<std::shared_ptr<Class>, std::string, std::string>;

    Model() = default;
    Model(const Model&) = default;
    Model(Model&&) noexcept = default;
    Model& operator=(const Model&) = default;
    Model& operator=(Model&&) noexcept = default;

    Model emptyCopy() const;
    void clearData();
    void add_class(const std::shared_ptr<Class>& cls);
    void add_classes(const std::vector<std::shared_ptr<Class>>& clsList);
    std::shared_ptr<Class> get_class(const std::string& name) const;
    bool hasClass(const std::string& name) const;

    bool is_side(const Side& s) const;
    bool isLang(const std::string& lang) const;

    // Работа с файлами
    void addFile(const std::shared_ptr<Class>& cls, const std::string& localPath, const std::string& content);

    // Получить подклассы по имени супер-класса
    std::vector<std::shared_ptr<Class>> getSubclassesOfClass(const std::string& superName) const;
    
    void create_std_types();
    
    bool is_simple_type(const std::string& type) const;
    
    bool is_skip(const Object& obj);

    // Удалить все классы, пришедшие из файла source_path. Заполняет removed именами классов.
    void remove_classes_from_source(const std::string& source_path,
                                    std::vector<std::string>* removed_names = nullptr,
                                    std::vector<std::pair<std::string,std::string>>* removed_name_group = nullptr);
public:
    // Парсер
    std::shared_ptr<Parser> parser; // указатель на конкретный парсер

    // Основные сущности
    std::vector<std::shared_ptr<Class>> classes;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Class>> classesForData;
    std::map<std::string, std::shared_ptr<Class>> classesDict;
    std::unordered_set<std::string> includes;

    // Конфигурация запуска (вместо отдельных флагов)
    Config configuration;
    Job config;
    std::vector<std::shared_ptr<FeatureGenerator>> feature_generators;

    // Простые типы
    static const std::unordered_set<std::string> simpleTypes;

    // Файлы и вывод
    std::unordered_map<std::string, std::string> out_dict;
    std::vector<FileEntry> files;
    std::vector<std::string> created_files;

    // Карта: путь исходного .mlc → имена/классы, распарсенные из него
    std::unordered_map<std::string, std::vector<std::string>> source_to_classnames;
    // Набор «грязных» классов для инкрементальной генерации. Пусто = генерить все
    std::unordered_set<std::string> dirty_classes;
};
