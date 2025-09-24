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
#include <array>
#include <functional>
#include "Class.hpp"
#include "Object.hpp"
#include "Function.hpp"

// Битовые флаги для форматов сериализации
enum class SerializeFormat {
    Xml = 1 << 0,
    Json = 1 << 1,
    
};
std::array<SerializeFormat, 2> SerializeFormat_getAll();
std::string SerializeFormat_to_str(SerializeFormat format);

inline int operator|(SerializeFormat a, SerializeFormat b) {
    return static_cast<int>(a) | static_cast<int>(b);
}

inline bool hasFlag(int flags, SerializeFormat f) {
    return (flags & static_cast<int>(f)) != 0;
}

// Возвращает все поддерживаемые форматы
inline std::vector<std::pair<SerializeFormat, std::string>> getAllSerializeFormats() {
    return {{SerializeFormat::Xml, "xml"}, {SerializeFormat::Json, "json"}};
}

class Parser;
class Model;

class CustomGenerator{
public:
    virtual void generate(Model& model) = 0;
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) = 0;
};

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
public:
    // Парсер
    std::shared_ptr<Parser> parser; // указатель на конкретный парсер

    // Основные сущности
    std::vector<std::shared_ptr<Class>> classes;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Class>> classesForData;
    std::map<std::string, std::shared_ptr<Class>> classesDict;
    std::unordered_set<std::string> includes;

    // Конфигурации и директории
    std::string model_name;
    std::vector<std::string> configs_directories;
    std::vector<std::string> data_directories;
    std::string out_data_directory;

    std::string out_directory;
    std::string language = "cpp";
    bool onlyData = false;
    std::string namespaceName = "mg";
    Side side = Side::both;
    bool phpValidate = true;
    bool allowDifferentVirtual = true;
    std::string testScript;
    std::string testScriptArgs;
    bool generateTests = false;
    bool generateIntrusive = true;
    bool generateFactory = true;
    std::function<bool(const std::string&)> filterCode;
    std::function<bool(const std::string&)> filterData;
    std::shared_ptr<CustomGenerator> customGenerator = nullptr;
    std::vector<SerializeFormat> serializeProtocol;
    bool joinToOneFile = false;
    bool autoRegistration = true;
    bool generateRefCounter = true;
    bool userIncludes = false;
    bool emptyMethods = false;
    bool auto_registration = false;

    // Простые типы
    static const std::unordered_set<std::string> simpleTypes;

    // Файлы и вывод
    std::unordered_map<std::string, std::string> out_dict;
    std::vector<FileEntry> files;
    std::vector<std::string> created_files;
    int serializeFormats = static_cast<int>(SerializeFormat::Xml) | static_cast<int>(SerializeFormat::Json);

    // Карта: путь исходного .mlc → имена/классы, распарсенные из него
    std::unordered_map<std::string, std::vector<std::string>> source_to_classnames;
    // Набор «грязных» классов для инкрементальной генерации. Пусто = генерить все
    std::unordered_set<std::string> dirty_classes;

    // Удалить все классы, пришедшие из файла source_path. Заполняет removed именами классов.
    void remove_classes_from_source(const std::string& source_path,
                                    std::vector<std::string>* removed_names = nullptr,
                                    std::vector<std::pair<std::string,std::string>>* removed_name_group = nullptr);
};
