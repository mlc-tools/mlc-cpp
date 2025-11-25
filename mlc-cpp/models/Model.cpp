//
//  Model.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Model.hpp"
#include "../models/Object.hpp"
#include "../utils/Config.hpp"
#include "Serialize.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

const std::unordered_set<std::string> Model::simpleTypes = {
    "int",  "float",    "bool",     "string", "int64_t",
    "uint", "unsigned", "uint64_t", "double"};

std::array<SerializeFormat, 2> SerializeFormat_getAll() {
    return std::array<SerializeFormat, 2>{
        SerializeFormat::Xml,
        SerializeFormat::Json,
    };
}

std::string SerializeFormat_to_str(SerializeFormat format) {
    if (format == SerializeFormat::Xml)
        return "xml";
    if (format == SerializeFormat::Json)
        return "json";
    assert(0);
    return "";
}

Model Model::emptyCopy() const {
    Model m = *this;
    m.classes.clear();
    m.classesForData.clear();
    m.classesDict.clear();
    m.includes.clear();
    return m;
}

// Очищает данные (парсер не трогать)
void Model::clearData() {
    parser = nullptr;
    classes.clear();
    classesForData.clear();
    classesDict.clear();
    out_dict.clear();
    files.clear();
    created_files.clear();
    source_to_classnames.clear();
    dirty_classes.clear();
}

// Добавление классов
void Model::add_class(const std::shared_ptr<Class> &cls) {
    if (cls->name == "Observable") {
        std::cout << "";
    }
    if (classesDict.count(cls->name) != 0) {
        auto iter =
            std::find_if(classes.begin(), classes.end(),
                         [cls](auto &c) { return c->name == cls->name; });
        if (iter != classes.end())
            classes.erase(iter);
    }

    classesDict[cls->name] = cls;
    classes.push_back(cls);

    std::sort(classes.begin(), classes.end(),
              [](const std::shared_ptr<Class> &lhs,
                 const std::shared_ptr<Class> &rhs) {
                  return lhs && rhs ? (lhs->group == rhs->group
                                           ? lhs->name < rhs->name
                                           : lhs->group < rhs->group)
                                    : lhs < rhs;
              });
}

void Model::add_classes(const std::vector<std::shared_ptr<Class>> &clsList) {
    for (auto &cls : clsList) {
        classesDict[cls->name] = cls;
        classes.push_back(cls);
        if (!cls->source_path.empty())
            source_to_classnames[cls->source_path].push_back(cls->name);
    }
}

// Поиск
std::shared_ptr<Class> Model::get_class(const std::string &name) const {
    auto it = classesDict.find(name);
    return it != classesDict.end() ? it->second : nullptr;
}
bool Model::hasClass(const std::string &name) const {
    return classesDict.find(name) != classesDict.end();
}

// Фильтры по стороне и языку
bool Model::is_side(const Side &s) const {
    return static_cast<int>(config.side) & static_cast<int>(s);
}
bool Model::isLang(const std::string &lang) const {
    return lang.empty() || lang == config.language;
}

// Работа с файлами
void Model::addFile(const std::shared_ptr<Class> &cls,
                    const std::string &localPath, const std::string &content) {
    if (cls && cls->name == "Observable") {
        std::cout << "";
    }

    if(cls){
        auto iter = std::find_if(files.begin(), files.end(), [cls, &localPath](auto &entry) {
            return std::get<0>(entry) ? std::get<0>(entry)->name == cls->name && std::get<1>(entry) == localPath : false;
        });
        assert(iter == files.end());
    }

    files.emplace_back(cls, localPath, content);
}

void Model::remove_classes_from_source(
    const std::string &source_path, std::vector<std::string> *removed_names,
    std::vector<std::pair<std::string, std::string>> *removed_name_group) {
    auto it = source_to_classnames.find(source_path);
    if (it == source_to_classnames.end())
        return;
    const auto &names = it->second;
    // remove from classes vector
    classes.erase(std::remove_if(classes.begin(), classes.end(),
                                 [&](const std::shared_ptr<Class> &c) {
                                     if (std::find(names.begin(), names.end(),
                                                   c->name) != names.end()) {
                                         classesDict.erase(c->name);
                                         if (removed_names)
                                             removed_names->push_back(c->name);
                                         if (removed_name_group)
                                             removed_name_group->emplace_back(
                                                 c->name, c->group);
                                         return true;
                                     }
                                     return false;
                                 }),
                  classes.end());
    source_to_classnames.erase(it);
}

// Получить подклассы по имени супер-класса
std::vector<std::shared_ptr<Class>>
Model::getSubclassesOfClass(const std::string &superName) const {
    std::vector<std::shared_ptr<Class>> result;
    for (auto &cls : classes) {
        if (cls->parent_class_name == superName ||
            (!cls->parent.expired() && cls->parent.lock()->name == superName)) {
            result.push_back(cls);
        }
    }
    return result;
}

void Model::create_std_types() {}

bool Model::is_simple_type(const std::string &type) const {
    return simpleTypes.count(type) > 0;
}

bool Model::is_skip(const Object &obj) const{
    if (!obj.lang_specific.empty() &&
        obj.lang_specific.count(config.language) == 0)
        return true;
    if (!is_side(obj.side))
        return true;
    return false;
}
