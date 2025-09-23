#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataStorage.h"
#include "data/DataEquipment.h"
#include "data/DataLadderLevels.h"
#include "data/DataLocale.h"
#include "data/DataMergeLevel.h"
#include "data/DataParams.h"
#include "ecs/DataLevel.h"
#include "ecs/DataStatUpgrade.h"
#include "ecs/DataUnit.h"
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
    : levels()
    , units()
    , stat_upgrades()
    , ladder_levelses()
    , locales()
    , paramses()
    , equipments()
    , merge_levels()
    , _loaded(false)
    , _reference_counter(1)
    {

    }

    DataStorage::~DataStorage()
    {
    }

    std::vector<std::string> DataStorage::get_levels_keys() const
    {
        std::vector<std::string> result;
        for (auto&& pair : this->levels)
        {
            auto& key = pair.first;
            auto& _ = pair.second;
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
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
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
            list_push(result, key);
        }
        return result;
    }

    std::vector<std::string> DataStorage::get_stat_upgrades_keys() const
    {
        std::vector<std::string> result;
        for (auto&& pair : this->stat_upgrades)
        {
            auto& key = pair.first;
            auto& _ = pair.second;
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
            list_push(result, key);
        }
        return result;
    }

    std::vector<std::string> DataStorage::get_ladder_levelses_keys() const
    {
        std::vector<std::string> result;
        for (auto&& pair : this->ladder_levelses)
        {
            auto& key = pair.first;
            auto& _ = pair.second;
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
            list_push(result, key);
        }
        return result;
    }

    std::vector<std::string> DataStorage::get_locales_keys() const
    {
        std::vector<std::string> result;
        for (auto&& pair : this->locales)
        {
            auto& key = pair.first;
            auto& _ = pair.second;
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
            list_push(result, key);
        }
        return result;
    }

    std::vector<std::string> DataStorage::get_paramses_keys() const
    {
        std::vector<std::string> result;
        for (auto&& pair : this->paramses)
        {
            auto& key = pair.first;
            auto& _ = pair.second;
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
            list_push(result, key);
        }
        return result;
    }

    std::vector<std::string> DataStorage::get_equipments_keys() const
    {
        std::vector<std::string> result;
        for (auto&& pair : this->equipments)
        {
            auto& key = pair.first;
            auto& _ = pair.second;
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
            list_push(result, key);
        }
        return result;
    }

    std::vector<std::string> DataStorage::get_merge_levels_keys() const
    {
        std::vector<std::string> result;
        for (auto&& pair : this->merge_levels)
        {
            auto& key = pair.first;
            auto& _ = pair.second;
            (void)key; //don't generate 'Unused variable' warning
            (void)_; //don't generate 'Unused variable' warning
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
        if(doc.root() != nullptr)
        {
            pugi::xml_node root = doc.root().first_child();

            for(auto& node : root.child("levels"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->levels.emplace(name, DataLevel());
            }

            for(auto& node : root.child("units"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->units.emplace(name, DataUnit());
            }

            for(auto& node : root.child("stat_upgrades"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->stat_upgrades.emplace(name, DataStatUpgrade());
            }

            for(auto& node : root.child("ladder_levelses"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->ladder_levelses.emplace(name, DataLadderLevels());
            }

            for(auto& node : root.child("locales"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->locales.emplace(name, DataLocale());
            }

            for(auto& node : root.child("paramses"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->paramses.emplace(name, DataParams());
            }

            for(auto& node : root.child("equipments"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->equipments.emplace(name, DataEquipment());
            }

            for(auto& node : root.child("merge_levels"))
            {
                auto name = node.attribute("key").as_string();
                non_const_this->merge_levels.emplace(name, DataMergeLevel());
            }

            DeserializerXml deserializer(root);
            non_const_this->_loaded = true;
            non_const_this->deserialize_xml(deserializer);
        }
    }

    template<>const DataLevel* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = levels.find(name);
            if(iter == levels.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::levels" << std::endl;
            }
            return iter != levels.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->levels[name];
    }

    template<>const DataUnit* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = units.find(name);
            if(iter == units.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::units" << std::endl;
            }
            return iter != units.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->units[name];
    }

    template<>const DataStatUpgrade* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = stat_upgrades.find(name);
            if(iter == stat_upgrades.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::stat_upgrades" << std::endl;
            }
            return iter != stat_upgrades.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->stat_upgrades[name];
    }

    template<>const DataLadderLevels* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = ladder_levelses.find(name);
            if(iter == ladder_levelses.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::ladder_levelses" << std::endl;
            }
            return iter != ladder_levelses.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->ladder_levelses[name];
    }

    template<>const DataLocale* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = locales.find(name);
            if(iter == locales.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::locales" << std::endl;
            }
            return iter != locales.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->locales[name];
    }

    template<>const DataParams* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = paramses.find(name);
            if(iter == paramses.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::paramses" << std::endl;
            }
            return iter != paramses.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->paramses[name];
    }

    template<>const DataEquipment* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = equipments.find(name);
            if(iter == equipments.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::equipments" << std::endl;
            }
            return iter != equipments.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->equipments[name];
    }

    template<>const DataMergeLevel* DataStorage::get(const std::string& name) const
    {
        if(name.empty())
        {
            return nullptr;
        }
        if(_loaded)
        {
            auto iter = merge_levels.find(name);
            if(iter == merge_levels.end())
            {
                std::cout << "Cannot find data with name [" << name << "] in DataStorage::merge_levels" << std::endl;
            }
            return iter != merge_levels.end() ? &iter->second : nullptr;
        }
        return &const_cast<DataStorage*>(this)->merge_levels[name];
    }
    const std::map<std::string, DataLevel>& DataStorage::get_levels() const
    {
        return this->levels;
    }

    const std::map<std::string, DataUnit>& DataStorage::get_units() const
    {
        return this->units;
    }

    const std::map<std::string, DataStatUpgrade>& DataStorage::get_stat_upgrades() const
    {
        return this->stat_upgrades;
    }

    const std::map<std::string, DataLadderLevels>& DataStorage::get_ladder_levelses() const
    {
        return this->ladder_levelses;
    }

    const std::map<std::string, DataLocale>& DataStorage::get_locales() const
    {
        return this->locales;
    }

    const std::map<std::string, DataParams>& DataStorage::get_paramses() const
    {
        return this->paramses;
    }

    const std::map<std::string, DataEquipment>& DataStorage::get_equipments() const
    {
        return this->equipments;
    }

    const std::map<std::string, DataMergeLevel>& DataStorage::get_merge_levels() const
    {
        return this->merge_levels;
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
        serializer.serialize(levels, "levels");
        serializer.serialize(units, "units");
        serializer.serialize(stat_upgrades, "stat_upgrades");
        serializer.serialize(ladder_levelses, "ladder_levelses");
        serializer.serialize(locales, "locales");
        serializer.serialize(paramses, "paramses");
        serializer.serialize(equipments, "equipments");
        serializer.serialize(merge_levels, "merge_levels");
    }

    void DataStorage::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(levels, "levels");
        deserializer.deserialize(units, "units");
        deserializer.deserialize(stat_upgrades, "stat_upgrades");
        deserializer.deserialize(ladder_levelses, "ladder_levelses");
        deserializer.deserialize(locales, "locales");
        deserializer.deserialize(paramses, "paramses");
        deserializer.deserialize(equipments, "equipments");
        deserializer.deserialize(merge_levels, "merge_levels");
    }

    void DataStorage::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(levels, "levels");
        serializer.serialize(units, "units");
        serializer.serialize(stat_upgrades, "stat_upgrades");
        serializer.serialize(ladder_levelses, "ladder_levelses");
        serializer.serialize(locales, "locales");
        serializer.serialize(paramses, "paramses");
        serializer.serialize(equipments, "equipments");
        serializer.serialize(merge_levels, "merge_levels");
    }

    void DataStorage::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(levels, "levels");
        deserializer.deserialize(units, "units");
        deserializer.deserialize(stat_upgrades, "stat_upgrades");
        deserializer.deserialize(ladder_levelses, "ladder_levelses");
        deserializer.deserialize(locales, "locales");
        deserializer.deserialize(paramses, "paramses");
        deserializer.deserialize(equipments, "equipments");
        deserializer.deserialize(merge_levels, "merge_levels");
    }

} //namespace mg
