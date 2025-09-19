//
//  GeneratorDataStorageCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorDataStorageCpp.hpp"
#include "Error.hpp"
//#include "SerializeFormat.hpp"
#include "Parser.hpp"
#include "Object.hpp"
#include "Function.hpp"


GeneratorDataStorageCpp::GeneratorDataStorageCpp()
  : GeneratorDataStorageBase()
{}

bool GeneratorDataStorageCpp::isNeedCreateStaticInstance() const {
    return false;
}

std::string GeneratorDataStorageCpp::getSharedMethodBody() {
    return R"(
    static DataStorage instance;
    return instance;
    )";
}

void GeneratorDataStorageCpp::createGetters(const std::vector<std::shared_ptr<Class>>& classes)
{
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
    const std::vector<std::shared_ptr<Class>>& classes,
    Function &getter)
{
    std::ostringstream impl;
    for (auto &cls : getStorageClasses()) {
        std::string dataName = getDataName(cls->name);
        std::string mapName  = getDataListName(dataName);

        impl << R"(
template<>
const )" << cls->name << R"(* DataStorage::get<)" << cls->name << R"(>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = )" << mapName << R"(.find(name);
        if (iter == )" << mapName << R"(.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::)" << mapName << R"(" << std::endl;
        }
        return iter != )" << mapName << R"(.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->)" << mapName << R"([name];
}
)";
    }
    getter.specific_implementations = impl.str();
}

std::vector<std::shared_ptr<Class>> GeneratorDataStorageCpp::getStorageClasses() const {
    std::vector<std::shared_ptr<Class>> result;
    for (auto &cls : _model->classes) {
        if (cls->is_storage && _model->is_side(cls->side)) {
            result.push_back(cls);
        }
    }
    return result;
}

std::string GeneratorDataStorageCpp::getInitializeFunctionJsonBody() {
    std::ostringstream body;
    body << R"(
Json::Value json;
Json::Reader reader;
reader.parse(content, json);
auto non_const_this = const_cast<DataStorage*>(this);
)";
    for (auto &cls : getStorageClasses()) {
        std::string dataName = getDataName(cls->name);
        std::string mapName  = getDataListName(dataName);

        body << R"(
auto )" << dataName << R"( = json[")" << mapName << R"("];
for (auto& node : )" << dataName << R"() {
    auto name = node["key"].asString();
    non_const_this->)" << mapName << R"(.emplace(name, )" << cls->name << R"(());
}
)";
    }
    body << R"(
DeserializerJson deserializer(json);
non_const_this->_loaded = true;
non_const_this->deserialize_json(deserializer);
)";
    return body.str();
}

std::string GeneratorDataStorageCpp::getInitializeFunctionXmlBody() {
    std::ostringstream body;
    body << R"(
pugi::xml_document doc;
doc.load_string(content.c_str());
auto non_const_this = const_cast<DataStorage*>(this);
if (doc.root() != nullptr) {
    pugi::xml_node root = doc.root().first_child();
)";
    for (auto &cls : getStorageClasses()) {
        std::string dataName = getDataName(cls->name);
        std::string mapName  = getDataListName(dataName);

        body << R"(
    for (auto& node : root.child(")" << mapName << R"(")) {
        auto name = node.attribute("key").as_string();
        non_const_this->)" << mapName << R"(.emplace(name, )" << cls->name << R"(());
    }
)";
    }
    body << R"(
    DeserializerXml deserializer(root);
    non_const_this->_loaded = true;
    non_const_this->deserialize_xml(deserializer);
}
)";
    return body.str();
}
