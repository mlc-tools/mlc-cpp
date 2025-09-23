#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BulletType.h"
#include "ComponentBase.h"
#include "DataUnit.h"
#include "DataUnitVisual.h"
#include "UnitStat.h"
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataUnit::TYPE("DataUnit");

    DataUnit::DataUnit()
    : name("")
    , bullet_type(BulletType::none)
    , visual()
    , stats()
    , stats_string()
    , components()
    , is_boss(false)
    , _reference_counter(1)
    {

    }

    DataUnit::~DataUnit()
    {
    }

    void DataUnit::retain()
    {
        ++this->_reference_counter;
    }

    int DataUnit::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool DataUnit::operator ==(const DataUnit& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->bullet_type == rhs.bullet_type;
        result = result && this->visual == rhs.visual;
        result = result && this->stats == rhs.stats;
        result = result && this->stats_string == rhs.stats_string;
        result = result && this->components == rhs.components;
        result = result && this->is_boss == rhs.is_boss;
        return result;
    }

    bool DataUnit::operator !=(const DataUnit& rhs) const
    {
        return !(*this == rhs);
    }

    DataUnit::DataUnit(const DataUnit& rhs)
    {
        this->operator=(rhs);
    }

    const DataUnit& DataUnit::operator =(const DataUnit& rhs)
    {
        this->name = rhs.name;
        this->bullet_type = rhs.bullet_type;
        this->visual = rhs.visual;
        this->stats = rhs.stats;
        this->stats_string = rhs.stats_string;
        this->components = rhs.components;
        this->is_boss = rhs.is_boss;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataUnit::get_type() const
    {
        return DataUnit::TYPE;
    }

    void DataUnit::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(bullet_type, "bullet_type");
        serializer.serialize(visual, "visual");
        serializer.serialize(stats, "stats");
        serializer.serialize(stats_string, "stats_string");
        serializer.serialize(components, "components");
        serializer.serialize(is_boss, "is_boss", bool(false));
    }

    void DataUnit::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(bullet_type, "bullet_type");
        deserializer.deserialize(visual, "visual");
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(stats_string, "stats_string");
        deserializer.deserialize(components, "components");
        deserializer.deserialize(is_boss, "is_boss", bool(false));
    }

    void DataUnit::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(bullet_type, "bullet_type");
        serializer.serialize(visual, "visual");
        serializer.serialize(stats, "stats");
        serializer.serialize(stats_string, "stats_string");
        serializer.serialize(components, "components");
        serializer.serialize(is_boss, "is_boss", bool(false));
    }

    void DataUnit::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(bullet_type, "bullet_type");
        deserializer.deserialize(visual, "visual");
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(stats_string, "stats_string");
        deserializer.deserialize(components, "components");
        deserializer.deserialize(is_boss, "is_boss", bool(false));
    }

} //namespace mg
