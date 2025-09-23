#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../data/EquipmentSlot.h"
#include "../ecs/ComponentBase.h"
#include "../ecs/UnitStat.h"
#include "ModelEquipment.h"
#include "ModelEquipmentSlot.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ModelEquipmentSlot::TYPE("ModelEquipmentSlot");

    ModelEquipmentSlot::ModelEquipmentSlot()
    : slot()
    , item(nullptr)
    , upgrade_level(0)
    , _reference_counter(1)
    {

    }

    ModelEquipmentSlot::~ModelEquipmentSlot()
    {
    }

    float ModelEquipmentSlot::get_stat(const UnitStat& stat, float base_value)
    {
        if(!this->item)
        {
            return base_value;
        }
        auto result = base_value;
        if(result == 0)
        {
            result = item->get_base_value(stat);
        }
        for(auto& level : item->data->merge_stats)
        {
            if(level.merge_level->index > this->item->merge_level->index)
            {
                break;
            }
            if(level.stat == stat)
            {
                result = level.modifier.modify(result);
            }
        }
        if(stat == this->item->data->main_stat)
        {
            result = result * (1 + 0.2 * this->upgrade_level);
        }
        return result;
    }

    std::string ModelEquipmentSlot::get_stat_string(const UnitStat& stat)
    {
        if(!this->item)
        {
            return "";
        }
        std::string result;
        for(auto& level : item->data->merge_stats)
        {
            if(level.merge_level > this->item->merge_level)
            {
                break;
            }
            if(level.stat == stat && !level.modifier.string_value.empty())
            {
                result = level.modifier.string_value;
            }
        }
        return result;
    }

    std::vector<intrusive_ptr<ComponentBase>> ModelEquipmentSlot::get_ecs_components()
    {
        std::vector<intrusive_ptr<ComponentBase>> result;
        if(!this->item)
        {
            return result;
        }

        for(auto& level : this->item->data->merge_stats)
        {
            if(level.merge_level->index > this->item->merge_level->index)
            {
                break;
            }

            if(level.component != nullptr)
            {
                list_push(result, clone_object<ComponentBase>(level.component));
            }
        }
        return result;
    }

    void ModelEquipmentSlot::retain()
    {
        ++this->_reference_counter;
    }

    int ModelEquipmentSlot::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool ModelEquipmentSlot::operator ==(const ModelEquipmentSlot& rhs) const
    {
        bool result = true;
        result = result && this->slot == rhs.slot;
        result = result && ((this->item == rhs.item) || (this->item != nullptr && rhs.item != nullptr && *this->item == *rhs.item));
        result = result && this->upgrade_level == rhs.upgrade_level;
        return result;
    }

    bool ModelEquipmentSlot::operator !=(const ModelEquipmentSlot& rhs) const
    {
        return !(*this == rhs);
    }

    ModelEquipmentSlot::ModelEquipmentSlot(const ModelEquipmentSlot& rhs)
    {
        this->operator=(rhs);
    }

    const ModelEquipmentSlot& ModelEquipmentSlot::operator =(const ModelEquipmentSlot& rhs)
    {
        this->slot = rhs.slot;
        this->item = rhs.item;
        this->upgrade_level = rhs.upgrade_level;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ModelEquipmentSlot::get_type() const
    {
        return ModelEquipmentSlot::TYPE;
    }

    void ModelEquipmentSlot::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(slot, "slot");
        serializer.serialize(item, "item");
        serializer.serialize(upgrade_level, "upgrade_level", int(0));
    }

    void ModelEquipmentSlot::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(slot, "slot");
        deserializer.deserialize(item, "item");
        deserializer.deserialize(upgrade_level, "upgrade_level", int(0));
    }

    void ModelEquipmentSlot::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(slot, "slot");
        serializer.serialize(item, "item");
        serializer.serialize(upgrade_level, "upgrade_level", int(0));
    }

    void ModelEquipmentSlot::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(slot, "slot");
        deserializer.deserialize(item, "item");
        deserializer.deserialize(upgrade_level, "upgrade_level", int(0));
    }

} //namespace mg
