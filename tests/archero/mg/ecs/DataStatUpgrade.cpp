#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "DataStatUpgrade.h"
#include "Modifier.h"
#include "UnitStat.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataStatUpgrade::TYPE("DataStatUpgrade");

    DataStatUpgrade::DataStatUpgrade()
    : name("")
    , rarity(SkillRarity::fine)
    , stats()
    , modifiers()
    , components()
    , to_hero(true)
    , to_spirits(false)
    , visual()
    , _reference_counter(1)
    {

    }

    DataStatUpgrade::~DataStatUpgrade()
    {
    }

    void DataStatUpgrade::retain()
    {
        ++this->_reference_counter;
    }

    int DataStatUpgrade::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataStatUpgrade::operator ==(const DataStatUpgrade& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->rarity == rhs.rarity;
        result = result && this->stats == rhs.stats;
        result = result && this->modifiers == rhs.modifiers;
        result = result && this->components == rhs.components;
        result = result && this->to_hero == rhs.to_hero;
        result = result && this->to_spirits == rhs.to_spirits;
        result = result && this->visual == rhs.visual;
        return result;
    }

    bool DataStatUpgrade::operator !=(const DataStatUpgrade& rhs) const
    {
        return !(*this == rhs);
    }

    DataStatUpgrade::DataStatUpgrade(const DataStatUpgrade& rhs)
    {
        this->operator=(rhs);
    }

    const DataStatUpgrade& DataStatUpgrade::operator =(const DataStatUpgrade& rhs)
    {
        this->name = rhs.name;
        this->rarity = rhs.rarity;
        this->stats = rhs.stats;
        this->modifiers = rhs.modifiers;
        this->components = rhs.components;
        this->to_hero = rhs.to_hero;
        this->to_spirits = rhs.to_spirits;
        this->visual = rhs.visual;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataStatUpgrade::get_type() const
    {
        return DataStatUpgrade::TYPE;
    }

    void DataStatUpgrade::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(rarity, "rarity");
        serializer.serialize(stats, "stats");
        serializer.serialize(modifiers, "modifiers");
        serializer.serialize(components, "components");
        serializer.serialize(to_hero, "to_hero", bool(true));
        serializer.serialize(to_spirits, "to_spirits", bool(false));
        serializer.serialize(visual, "visual");
    }

    void DataStatUpgrade::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(rarity, "rarity");
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(modifiers, "modifiers");
        deserializer.deserialize(components, "components");
        deserializer.deserialize(to_hero, "to_hero", bool(true));
        deserializer.deserialize(to_spirits, "to_spirits", bool(false));
        deserializer.deserialize(visual, "visual");
    }

    void DataStatUpgrade::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(rarity, "rarity");
        serializer.serialize(stats, "stats");
        serializer.serialize(modifiers, "modifiers");
        serializer.serialize(components, "components");
        serializer.serialize(to_hero, "to_hero", bool(true));
        serializer.serialize(to_spirits, "to_spirits", bool(false));
        serializer.serialize(visual, "visual");
    }

    void DataStatUpgrade::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(rarity, "rarity");
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(modifiers, "modifiers");
        deserializer.deserialize(components, "components");
        deserializer.deserialize(to_hero, "to_hero", bool(true));
        deserializer.deserialize(to_spirits, "to_spirits", bool(false));
        deserializer.deserialize(visual, "visual");
    }

} //namespace mg
