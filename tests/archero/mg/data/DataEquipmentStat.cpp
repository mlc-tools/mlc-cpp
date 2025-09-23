#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../ecs/ComponentBase.h"
#include "../ecs/Modifier.h"
#include "../ecs/UnitStat.h"
#include "DataEquipmentStat.h"
#include "DataMergeLevel.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataEquipmentStat::TYPE("DataEquipmentStat");

    DataEquipmentStat::DataEquipmentStat()
    : merge_level(nullptr)
    , stat()
    , modifier()
    , desc("")
    , component(nullptr)
    , is_shown_in_ui(true)
    , _reference_counter(1)
    {

    }

    DataEquipmentStat::~DataEquipmentStat()
    {
    }

    void DataEquipmentStat::retain()
    {
        ++this->_reference_counter;
    }

    int DataEquipmentStat::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool DataEquipmentStat::operator ==(const DataEquipmentStat& rhs) const
    {
        bool result = true;
        result = result && ((this->merge_level == rhs.merge_level) || (this->merge_level != nullptr && rhs.merge_level != nullptr && *this->merge_level == *rhs.merge_level));
        result = result && this->stat == rhs.stat;
        result = result && this->modifier == rhs.modifier;
        result = result && this->desc == rhs.desc;
        result = result && ((this->component == rhs.component) || (this->component != nullptr && rhs.component != nullptr && *this->component == *rhs.component));
        result = result && this->is_shown_in_ui == rhs.is_shown_in_ui;
        return result;
    }

    bool DataEquipmentStat::operator !=(const DataEquipmentStat& rhs) const
    {
        return !(*this == rhs);
    }

    DataEquipmentStat::DataEquipmentStat(const DataEquipmentStat& rhs)
    {
        this->operator=(rhs);
    }

    const DataEquipmentStat& DataEquipmentStat::operator =(const DataEquipmentStat& rhs)
    {
        this->merge_level = rhs.merge_level;
        this->stat = rhs.stat;
        this->modifier = rhs.modifier;
        this->desc = rhs.desc;
        this->component = rhs.component;
        this->is_shown_in_ui = rhs.is_shown_in_ui;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataEquipmentStat::get_type() const
    {
        return DataEquipmentStat::TYPE;
    }

    void DataEquipmentStat::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(merge_level, "merge_level");
        serializer.serialize(stat, "stat");
        serializer.serialize(modifier, "modifier");
        serializer.serialize(desc, "desc", std::string(""));
        serializer.serialize(component, "component");
        serializer.serialize(is_shown_in_ui, "is_shown_in_ui", bool(true));
    }

    void DataEquipmentStat::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(merge_level, "merge_level");
        deserializer.deserialize(stat, "stat");
        deserializer.deserialize(modifier, "modifier");
        deserializer.deserialize(desc, "desc", std::string(""));
        deserializer.deserialize(component, "component");
        deserializer.deserialize(is_shown_in_ui, "is_shown_in_ui", bool(true));
    }

    void DataEquipmentStat::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(merge_level, "merge_level");
        serializer.serialize(stat, "stat");
        serializer.serialize(modifier, "modifier");
        serializer.serialize(desc, "desc", std::string(""));
        serializer.serialize(component, "component");
        serializer.serialize(is_shown_in_ui, "is_shown_in_ui", bool(true));
    }

    void DataEquipmentStat::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(merge_level, "merge_level");
        deserializer.deserialize(stat, "stat");
        deserializer.deserialize(modifier, "modifier");
        deserializer.deserialize(desc, "desc", std::string(""));
        deserializer.deserialize(component, "component");
        deserializer.deserialize(is_shown_in_ui, "is_shown_in_ui", bool(true));
    }

} //namespace mg
