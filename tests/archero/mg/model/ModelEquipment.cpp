#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../data/DataEquipment.h"
#include "../data/DataMergeLevel.h"
#include "../ecs/UnitStat.h"
#include "Model.h"
#include "ModelEquipment.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ModelEquipment::TYPE("ModelEquipment");

    ModelEquipment::ModelEquipment()
    : data(nullptr)
    , merge_level(nullptr)
    , equip(false)
    , _reference_counter(1)
    {

    }

    ModelEquipment::~ModelEquipment()
    {
    }

    float ModelEquipment::get_base_value(const UnitStat& stat)
    {
        float result = 0;
        if(in_map(stat, data->stats))
        {
            result = data->stats.at(stat);
        }
        else
        {
            for(auto& level : this->data->merge_stats)
            {
                if(level.merge_level->index > 1)
                {
                    break;
                }
                if(level.stat == stat)
                {
                    result = level.modifier.modify(result);
                }
            }
        }
        if(stat == this->data->main_stat)
        {
            auto level = this->merge_level->index-1;
            result = result * (1 + 0.2*level);
        }
        return result;
    }

    void ModelEquipment::retain()
    {
        ++this->_reference_counter;
    }

    int ModelEquipment::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool ModelEquipment::operator ==(const ModelEquipment& rhs) const
    {
        bool result = true;
        result = result && ((this->data == rhs.data) || (this->data != nullptr && rhs.data != nullptr && *this->data == *rhs.data));
        result = result && ((this->merge_level == rhs.merge_level) || (this->merge_level != nullptr && rhs.merge_level != nullptr && *this->merge_level == *rhs.merge_level));
        result = result && this->equip == rhs.equip;
        return result;
    }

    bool ModelEquipment::operator !=(const ModelEquipment& rhs) const
    {
        return !(*this == rhs);
    }

    ModelEquipment::ModelEquipment(const ModelEquipment& rhs)
    {
        this->operator=(rhs);
    }

    const ModelEquipment& ModelEquipment::operator =(const ModelEquipment& rhs)
    {
        this->data = rhs.data;
        this->merge_level = rhs.merge_level;
        this->equip = rhs.equip;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ModelEquipment::get_type() const
    {
        return ModelEquipment::TYPE;
    }

    void ModelEquipment::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(data, "data");
        serializer.serialize(merge_level, "merge_level");
        serializer.serialize(equip, "equip", bool(false));
    }

    void ModelEquipment::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(data, "data");
        deserializer.deserialize(merge_level, "merge_level");
        deserializer.deserialize(equip, "equip", bool(false));
    }

    void ModelEquipment::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(data, "data");
        serializer.serialize(merge_level, "merge_level");
        serializer.serialize(equip, "equip", bool(false));
    }

    void ModelEquipment::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(data, "data");
        deserializer.deserialize(merge_level, "merge_level");
        deserializer.deserialize(equip, "equip", bool(false));
    }

} //namespace mg
