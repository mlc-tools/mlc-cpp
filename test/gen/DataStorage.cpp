#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataAdd.h"
#include "DataComplexMap.h"
#include "DataListListBool.h"
#include "DataStorage.h"
#include "data/DataDict.h"
#include "data/DataNonExist.h"
#include "data/DataUnit.h"
#include "data/LadderLevels.h"
#include <map>
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string DataStorage::TYPE("DataStorage");

    
    DataStorage::DataStorage()
    : complex_maps()
, list_list_bools()
, non_exists()
, ladder_levelss()
, units()
, dicts()
, adds()
, _loaded(false)
, _reference_counter(1)
{
    
    }

    DataStorage::~DataStorage(){}


std::vector<std::string> DataStorage::get_complex_maps_keys() const
{
std::vector<std::string> result;
for (auto&& pair : this->complex_maps)
{
    auto& key = pair.first;
    auto& _ = pair.second;
    (void)key; // don't generate 'Unused variable' warning
    (void)_; // don't generate 'Unused variable' warning
    list_push(result, key);
}
return result;
}

std::vector<std::string> DataStorage::get_list_list_bools_keys() const
{
std::vector<std::string> result;
for (auto&& pair : this->list_list_bools)
{
    auto& key = pair.first;
    auto& _ = pair.second;
    (void)key; // don't generate 'Unused variable' warning
    (void)_; // don't generate 'Unused variable' warning
    list_push(result, key);
}
return result;
}

std::vector<std::string> DataStorage::get_non_exists_keys() const
{
std::vector<std::string> result;
for (auto&& pair : this->non_exists)
{
    auto& key = pair.first;
    auto& _ = pair.second;
    (void)key; // don't generate 'Unused variable' warning
    (void)_; // don't generate 'Unused variable' warning
    list_push(result, key);
}
return result;
}

std::vector<std::string> DataStorage::get_ladder_levelss_keys() const
{
std::vector<std::string> result;
for (auto&& pair : this->ladder_levelss)
{
    auto& key = pair.first;
    auto& _ = pair.second;
    (void)key; // don't generate 'Unused variable' warning
    (void)_; // don't generate 'Unused variable' warning
    list_push(result, key);
}
return result;
}

std::vector<std::string> DataStorage::get_units_keys() const
{
std::vector<std::string> result;
for (auto&& pair : this->units)
{
    auto& key = pair.first;
    auto& _ = pair.second;
    (void)key; // don't generate 'Unused variable' warning
    (void)_; // don't generate 'Unused variable' warning
    list_push(result, key);
}
return result;
}

std::vector<std::string> DataStorage::get_dicts_keys() const
{
std::vector<std::string> result;
for (auto&& pair : this->dicts)
{
    auto& key = pair.first;
    auto& _ = pair.second;
    (void)key; // don't generate 'Unused variable' warning
    (void)_; // don't generate 'Unused variable' warning
    list_push(result, key);
}
return result;
}

std::vector<std::string> DataStorage::get_adds_keys() const
{
std::vector<std::string> result;
for (auto&& pair : this->adds)
{
    auto& key = pair.first;
    auto& _ = pair.second;
    (void)key; // don't generate 'Unused variable' warning
    (void)_; // don't generate 'Unused variable' warning
    list_push(result, key);
}
return result;
}

const DataStorage& DataStorage::shared()
{

    static DataStorage instance;
    return instance;
    
}

void DataStorage::initialize_xml(const std::string& content) const
{

pugi::xml_document doc;
doc.load_string(content.c_str());
auto non_const_this = const_cast<DataStorage*>(this);
if (doc.root() != nullptr) {
    pugi::xml_node root = doc.root().first_child();

    for (auto& node : root.child("complex_maps")) {
        auto name = node.attribute("key").as_string();
        non_const_this->complex_maps.emplace(name, DataComplexMap());
    }

    for (auto& node : root.child("list_list_bools")) {
        auto name = node.attribute("key").as_string();
        non_const_this->list_list_bools.emplace(name, DataListListBool());
    }

    for (auto& node : root.child("non_exists")) {
        auto name = node.attribute("key").as_string();
        non_const_this->non_exists.emplace(name, DataNonExist());
    }

    for (auto& node : root.child("ladder_levelss")) {
        auto name = node.attribute("key").as_string();
        non_const_this->ladder_levelss.emplace(name, LadderLevels());
    }

    for (auto& node : root.child("units")) {
        auto name = node.attribute("key").as_string();
        non_const_this->units.emplace(name, DataUnit());
    }

    for (auto& node : root.child("dicts")) {
        auto name = node.attribute("key").as_string();
        non_const_this->dicts.emplace(name, DataDict());
    }

    for (auto& node : root.child("adds")) {
        auto name = node.attribute("key").as_string();
        non_const_this->adds.emplace(name, DataAdd());
    }

    DeserializerXml deserializer(root);
    non_const_this->_loaded = true;
    non_const_this->deserialize_xml(deserializer);
}

}


template<>
const DataComplexMap* DataStorage::get<DataComplexMap>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = complex_maps.find(name);
        if (iter == complex_maps.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::complex_maps" << std::endl;
        }
        return iter != complex_maps.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->complex_maps[name];
}

template<>
const DataListListBool* DataStorage::get<DataListListBool>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = list_list_bools.find(name);
        if (iter == list_list_bools.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::list_list_bools" << std::endl;
        }
        return iter != list_list_bools.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->list_list_bools[name];
}

template<>
const DataNonExist* DataStorage::get<DataNonExist>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = non_exists.find(name);
        if (iter == non_exists.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::non_exists" << std::endl;
        }
        return iter != non_exists.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->non_exists[name];
}

template<>
const LadderLevels* DataStorage::get<LadderLevels>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = ladder_levelss.find(name);
        if (iter == ladder_levelss.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::ladder_levelss" << std::endl;
        }
        return iter != ladder_levelss.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->ladder_levelss[name];
}

template<>
const DataUnit* DataStorage::get<DataUnit>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = units.find(name);
        if (iter == units.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::units" << std::endl;
        }
        return iter != units.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->units[name];
}

template<>
const DataDict* DataStorage::get<DataDict>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = dicts.find(name);
        if (iter == dicts.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::dicts" << std::endl;
        }
        return iter != dicts.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->dicts[name];
}

template<>
const DataAdd* DataStorage::get<DataAdd>(const std::string& name) const {
    if (name.empty()) {
        return nullptr;
    }
    if (_loaded) {
        auto iter = adds.find(name);
        if (iter == adds.end()) {
            std::cout << "Cannot find data with name [" << name << "] in DataStorage::adds" << std::endl;
        }
        return iter != adds.end() ? &iter->second : nullptr;
    }
    return &const_cast<DataStorage*>(this)->adds[name];
}
const std::map<std::string, DataComplexMap>& DataStorage::get_complex_maps() const
{
return this->complex_maps;
}

const std::map<std::string, DataListListBool>& DataStorage::get_list_list_bools() const
{
return this->list_list_bools;
}

const std::map<std::string, DataNonExist>& DataStorage::get_non_exists() const
{
return this->non_exists;
}

const std::map<std::string, LadderLevels>& DataStorage::get_ladder_levelss() const
{
return this->ladder_levelss;
}

const std::map<std::string, DataUnit>& DataStorage::get_units() const
{
return this->units;
}

const std::map<std::string, DataDict>& DataStorage::get_dicts() const
{
return this->dicts;
}

const std::map<std::string, DataAdd>& DataStorage::get_adds() const
{
return this->adds;
}

void DataStorage::retain()
{
++this->_reference_counter;
}

int DataStorage::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

std::string DataStorage::get_type() const
{
return DataStorage::TYPE;
}

void DataStorage::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(complex_maps, "complex_maps");
serializer.serialize(list_list_bools, "list_list_bools");
serializer.serialize(non_exists, "non_exists");
serializer.serialize(ladder_levelss, "ladder_levelss");
serializer.serialize(units, "units");
serializer.serialize(dicts, "dicts");
serializer.serialize(adds, "adds");
}

void DataStorage::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(complex_maps, "complex_maps");
deserializer.deserialize(list_list_bools, "list_list_bools");
deserializer.deserialize(non_exists, "non_exists");
deserializer.deserialize(ladder_levelss, "ladder_levelss");
deserializer.deserialize(units, "units");
deserializer.deserialize(dicts, "dicts");
deserializer.deserialize(adds, "adds");
}

void DataStorage::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(complex_maps, "complex_maps");
serializer.serialize(list_list_bools, "list_list_bools");
serializer.serialize(non_exists, "non_exists");
serializer.serialize(ladder_levelss, "ladder_levelss");
serializer.serialize(units, "units");
serializer.serialize(dicts, "dicts");
serializer.serialize(adds, "adds");
}

void DataStorage::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(complex_maps, "complex_maps");
deserializer.deserialize(list_list_bools, "list_list_bools");
deserializer.deserialize(non_exists, "non_exists");
deserializer.deserialize(ladder_levelss, "ladder_levelss");
deserializer.deserialize(units, "units");
deserializer.deserialize(dicts, "dicts");
deserializer.deserialize(adds, "adds");
}

} // namespace mg
