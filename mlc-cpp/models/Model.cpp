//
//  Model.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Model.hpp"
#include "../models/Object.hpp"
#include <cassert>
#include <iostream>

const std::unordered_set<std::string> Model::simpleTypes = {
    "int",
    "float",
    "bool",
    "string",
    "int64_t",
    "uint",
    "unsigned",
    "uint64_t",
    "double"
};

std::array<SerializeFormat, 2> SerializeFormat_getAll() {
    return std::array<SerializeFormat, 2>{
        SerializeFormat::Xml,
        SerializeFormat::Json,
    };
}

std::string SerializeFormat_to_str(SerializeFormat format){
    if(format == SerializeFormat::Xml) return "xml";
    if(format == SerializeFormat::Json) return "json";
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
}

// Добавление классов
void Model::add_class(const std::shared_ptr<Class>& cls) {
    if(cls->name == "Observable"){
        std::cout << "";
    }
    classesDict[cls->name] = cls;
    classes.push_back(cls);
}

void Model::add_classes(const std::vector<std::shared_ptr<Class>>& clsList) {
    for (auto& cls : clsList) {
        classesDict[cls->name] = cls;
        classes.push_back(cls);
    }
}

// Поиск
std::shared_ptr<Class> Model::get_class(const std::string& name) const {
    auto it = classesDict.find(name);
    return it != classesDict.end() ? it->second : nullptr;
}
bool Model::hasClass(const std::string& name) const {
    return classesDict.find(name) != classesDict.end();
}

// Фильтры по стороне и языку
bool Model::is_side(const Side& s) const {
    return static_cast<int>(side) & static_cast<int>(s);
}
bool Model::isLang(const std::string& lang) const {
    return lang.empty() || lang == language;
}

// Работа с файлами
void Model::addFile(const std::shared_ptr<Class>& cls, const std::string& localPath, const std::string& content) {
    if(cls && cls->name == "Observable"){
        std::cout << "";
    }

    files.emplace_back(cls, localPath, content);
}

// Получить подклассы по имени супер-класса
std::vector<std::shared_ptr<Class>> Model::getSubclassesOfClass(const std::string& superName) const {
    std::vector<std::shared_ptr<Class>> result;
    for (auto& cls : classes) {
        if (cls->parent_class_name == superName || (!cls->parent.expired() && cls->parent.lock()->name == superName)) {
            result.push_back(cls);
        }
    }
    return result;
}

void Model::create_std_types()
{
}

bool Model::is_simple_type(const std::string& type) const{
    return simpleTypes.count(type) > 0;
}

bool Model::is_skip(const Object& obj) {
    if(!obj.lang_specific.empty() && obj.lang_specific.count(language) == 0)
        return true;
    if(!is_side(obj.side))
        return true;
    return false;
}
