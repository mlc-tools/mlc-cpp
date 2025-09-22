#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../ecs/UnitStat.h"
#include "DataEquipment.h"
#include "DataEquipmentStat.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataEquipment::TYPE("DataEquipment");

    DataEquipment::DataEquipment()
    : visual()
    , name("")
    , stats()
    , main_stat(UnitStat::none)
    , slot(EquipmentSlot::weapon)
    , merge_stats()
    , _reference_counter(1)
    {

    }

    DataEquipment::~DataEquipment()
    {
    }

    void DataEquipment::retain()
    {
        ++this->_reference_counter;
    }

    int DataEquipment::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataEquipment::operator ==(const DataEquipment& rhs) const
    {
        bool result = true;
        result = result && this->visual == rhs.visual;
        result = result && this->name == rhs.name;
        result = result && this->stats == rhs.stats;
        result = result && this->main_stat == rhs.main_stat;
        result = result && this->slot == rhs.slot;
        result = result && this->merge_stats == rhs.merge_stats;
        return result;
    }

    bool DataEquipment::operator !=(const DataEquipment& rhs) const
    {
        return !(*this == rhs);
    }

    DataEquipment::DataEquipment(const DataEquipment& rhs)
    {
        this->operator=(rhs);
    }

    const DataEquipment& DataEquipment::operator =(const DataEquipment& rhs)
    {
        this->visual = rhs.visual;
        this->name = rhs.name;
        this->stats = rhs.stats;
        this->main_stat = rhs.main_stat;
        this->slot = rhs.slot;
        this->merge_stats = rhs.merge_stats;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataEquipment::get_type() const
    {
        return DataEquipment::TYPE;
    }

    void DataEquipment::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(visual, "visual");
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(stats, "stats");
        serializer.serialize(main_stat, "main_stat");
        serializer.serialize(slot, "slot");
        serializer.serialize(merge_stats, "merge_stats");
    }

    void DataEquipment::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(visual, "visual");
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(main_stat, "main_stat");
        deserializer.deserialize(slot, "slot");
        deserializer.deserialize(merge_stats, "merge_stats");
    }

    void DataEquipment::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(visual, "visual");
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(stats, "stats");
        serializer.serialize(main_stat, "main_stat");
        serializer.serialize(slot, "slot");
        serializer.serialize(merge_stats, "merge_stats");
    }

    void DataEquipment::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(visual, "visual");
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(main_stat, "main_stat");
        deserializer.deserialize(slot, "slot");
        deserializer.deserialize(merge_stats, "merge_stats");
    }

} //namespace mg
