//
//  GeneratorDataStorageCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorDataStorageCpp.hpp"
#include "../core/Parser.hpp"
#include "../models/Function.hpp"
#include "../models/Object.hpp"
#include "Error.hpp"

GeneratorDataStorageCpp::GeneratorDataStorageCpp(const FeatureDataStorage &config)
: GeneratorDataStorageBase(config) {
    
}

bool GeneratorDataStorageCpp::isNeedCreateStaticInstance() const {
    return false;
}

std::string GeneratorDataStorageCpp::getSharedMethodBody() {
    return R"(static DataStorage instance;
    return instance;)";
}

void GeneratorDataStorageCpp::createGetters(
    const std::vector<std::shared_ptr<Class>> &classes) {
    // Создаём шаблонный метод get<T>
    Function getter;
    getter.name = "get";
    getter.is_template = true;
    getter.return_type.type = "template <class T> const T*";
    getter.is_const = true;
    getter.callable_args.emplace_back(Object("string", "name"));

    // Регистрируем в списке методов
    _class->functions.push_back(getter);

    // Добавляем реализации специализаций
    generateImplementations(classes, _class->functions.back());
}

void GeneratorDataStorageCpp::generateImplementations(
    const std::vector<std::shared_ptr<Class>> & /*classes*/, Function &getter) {
    std::string impl;
    for (auto &cls : getStorageClasses()) {
        std::string dataName = getDataName(cls->name);
        std::string mapName = getDataListName(dataName);

        impl += ::format(
            R"(
template<>const {cls}* DataStorage::get(const std::string& name) const 
{
    if(name.empty()) 
    {
        return nullptr;
    }
    if(_loaded) 
    {
        auto iter = {map}.find(name);
        if(iter == {map}.end()) 
        {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::{map}" << std::endl;
        }
        return iter != {map}.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->{map}[name];
}
)",
            {{"cls", cls->name}, {"map", mapName}});
    }
    getter.specific_implementations = impl;
}

std::vector<std::shared_ptr<Class>>
GeneratorDataStorageCpp::getStorageClasses() const {
    std::vector<std::shared_ptr<Class>> result;
    for (auto &cls : _model->classes) {
        if (cls->is_storage && _model->is_side(cls->side)) {
            result.push_back(cls);
        }
    }
    return result;
}

std::string GeneratorDataStorageCpp::getInitializeFunctionJsonBody() {
    std::string body;
    body += R"(
Json::Value json;
Json::Reader reader;
reader.parse(content, json);
auto non_const_this = const_cast<DataStorage*>(this);
)";
    for (auto &cls : getStorageClasses()) {
        std::string dataName = getDataName(cls->name);
        std::string mapName = getDataListName(dataName);

        body += ::format(
            R"(
auto {data} = json["{map}"];
for (auto& node : {data}) 
{
    auto name = node["key"].asString();
    non_const_this->{map}.emplace(name, {cls}());
}
)",
            {{"data", dataName}, {"map", mapName}, {"cls", cls->name}});
    }
    body += R"(
DeserializerJson deserializer(json);
non_const_this->_loaded = true;
non_const_this->deserialize_json(deserializer);
)";
    return body;
}

std::string GeneratorDataStorageCpp::getInitializeFunctionXmlBody() {
    std::string body;
    body += R"(pugi::xml_document doc;
doc.load_string(content.c_str());
auto non_const_this = const_cast<DataStorage*>(this);
if(doc.root() != nullptr) 
{
    pugi::xml_node root = doc.root().first_child();
)";
    for (auto &cls : getStorageClasses()) {
        std::string dataName = getDataName(cls->name);
        std::string mapName = getDataListName(dataName);

        body += ::format(
            R"(
    for(auto& node : root.child("{map}")) 
    {
        auto name = node.attribute("key").as_string();
        non_const_this->{map}.emplace(name, {cls}());
    }
)",
            {{"map", mapName}, {"cls", cls->name}});
    }
    body += R"(
    DeserializerXml deserializer(root);
    non_const_this->_loaded = true;
    non_const_this->deserialize_xml(deserializer);
}
)";
    return body;
}
