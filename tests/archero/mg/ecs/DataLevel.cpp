#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataLevel.h"
#include "DataWaveBase.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataLevel::TYPE("DataLevel");

    DataLevel::DataLevel()
    : name("")
    , tmx_maps()
    , waves()
    , enemies_hp_rate(0.0f)
    , enemies_damage_rate(0.0f)
    , _reference_counter(1)
    {

    }

    DataLevel::~DataLevel()
    {
    }

    bool DataLevel::is_change_view_on_next_wave() const
    {
        return this->tmx_maps.size() > 0;
    }

    void DataLevel::retain()
    {
        ++this->_reference_counter;
    }

    int DataLevel::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool DataLevel::operator ==(const DataLevel& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->tmx_maps == rhs.tmx_maps;
        result = result && this->waves == rhs.waves;
        result = result && this->enemies_hp_rate == rhs.enemies_hp_rate;
        result = result && this->enemies_damage_rate == rhs.enemies_damage_rate;
        return result;
    }

    bool DataLevel::operator !=(const DataLevel& rhs) const
    {
        return !(*this == rhs);
    }

    DataLevel::DataLevel(const DataLevel& rhs)
    {
        this->operator=(rhs);
    }

    const DataLevel& DataLevel::operator =(const DataLevel& rhs)
    {
        this->name = rhs.name;
        this->tmx_maps = rhs.tmx_maps;
        this->waves = rhs.waves;
        this->enemies_hp_rate = rhs.enemies_hp_rate;
        this->enemies_damage_rate = rhs.enemies_damage_rate;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataLevel::get_type() const
    {
        return DataLevel::TYPE;
    }

    void DataLevel::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(tmx_maps, "tmx_maps");
        serializer.serialize(waves, "waves");
        serializer.serialize(enemies_hp_rate, "enemies_hp_rate", float(0.0f));
        serializer.serialize(enemies_damage_rate, "enemies_damage_rate", float(0.0f));
    }

    void DataLevel::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(tmx_maps, "tmx_maps");
        deserializer.deserialize(waves, "waves");
        deserializer.deserialize(enemies_hp_rate, "enemies_hp_rate", float(0.0f));
        deserializer.deserialize(enemies_damage_rate, "enemies_damage_rate", float(0.0f));
    }

    void DataLevel::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(tmx_maps, "tmx_maps");
        serializer.serialize(waves, "waves");
        serializer.serialize(enemies_hp_rate, "enemies_hp_rate", float(0.0f));
        serializer.serialize(enemies_damage_rate, "enemies_damage_rate", float(0.0f));
    }

    void DataLevel::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(tmx_maps, "tmx_maps");
        deserializer.deserialize(waves, "waves");
        deserializer.deserialize(enemies_hp_rate, "enemies_hp_rate", float(0.0f));
        deserializer.deserialize(enemies_damage_rate, "enemies_damage_rate", float(0.0f));
    }

} //namespace mg
