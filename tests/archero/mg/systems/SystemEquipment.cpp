#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../Observable.h"
#include "../data/DataEquipment.h"
#include "../data/DataMergeLevel.h"
#include "../data/EquipmentSlot.h"
#include "../ecs/UnitStat.h"
#include "../model/Model.h"
#include "../model/ModelEquipment.h"
#include "../model/ModelEquipmentSlot.h"
#include "../model/ModelUser.h"
#include "../model/System.h"
#include "SystemEquipment.h"
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemEquipment::TYPE("SystemEquipment");

    SystemEquipment::SystemEquipment()
    : slots()
    , items()
    , event_inventory_changed()
    , event_slot_changed()
    {

    }

    SystemEquipment::~SystemEquipment()
    {
    }

    void SystemEquipment::initialize(ModelUser* user)
    {
        if(map_size(this->slots) == 0)
        {
            this->initialize_slot(EquipmentSlot::weapon);
            this->initialize_slot(EquipmentSlot::ring);
            this->initialize_slot(EquipmentSlot::amulet);
            this->initialize_slot(EquipmentSlot::armor);
            this->initialize_slot(EquipmentSlot::head);
            this->initialize_slot(EquipmentSlot::boots);

            if(this->items.empty())
            {
                auto item = DataStorage::shared().get<DataEquipment>("weapon_bow_1");
                auto normal_merge = DataStorage::shared().get<DataMergeLevel>("normal");
                auto default_weapon = add_item(item, normal_merge);
                this->equip(default_weapon);

                add_item(DataStorage::shared().get<DataEquipment>("ring_1"), normal_merge);
                add_item(DataStorage::shared().get<DataEquipment>("amulet_1"), normal_merge);
                add_item(DataStorage::shared().get<DataEquipment>("body_1"), normal_merge);
                add_item(DataStorage::shared().get<DataEquipment>("legs_1"), normal_merge);
                add_item(DataStorage::shared().get<DataEquipment>("head_1"), normal_merge);
            }
        }
    }

    void SystemEquipment::initialize_slot(EquipmentSlot slot)
    {
        this->slots[slot] = make_intrusive<ModelEquipmentSlot>();
        this->slots[slot]->slot = slot;
    }

    intrusive_ptr<ModelEquipment> SystemEquipment::add_item(const DataEquipment* data, const DataMergeLevel* merge_level)
    {
        auto model = make_intrusive<ModelEquipment>();
        model->data = data;
        model->merge_level = merge_level;
        list_push(this->items, model);
        this->event_inventory_changed.notify();
        return model;
    }

    void SystemEquipment::equip(ModelEquipment* model)
    {
        auto slot = model->data->slot;
        if(this->slots[slot]->item)
        {
            unequip(this->slots[slot]->item);
        }

        this->slots[slot]->item = model;
        model->equip = true;
        this->event_slot_changed.notify(slot);
    }

    void SystemEquipment::unequip(ModelEquipment* model)
    {
        auto slot = model->data->slot;
        this->slots[slot]->item = nullptr;
        model->equip = false;
        this->event_slot_changed.notify(slot);
    }

    float SystemEquipment::get_stat(const UnitStat& stat, float base_value)
    {
        auto result = base_value;
        for (auto&& pair : this->slots)
        {
            auto& slot = pair.first;
            auto& model = pair.second;
            (void)slot; //don't generate 'Unused variable' warning
            (void)model; //don't generate 'Unused variable' warning
            result = model->get_stat(stat, result);
        }
        return result;
    }

    std::string SystemEquipment::get_stat_string(const UnitStat& stat)
    {
        std::string result;
        for (auto&& pair : this->slots)
        {
            auto& slot = pair.first;
            auto& model = pair.second;
            (void)slot; //don't generate 'Unused variable' warning
            (void)model; //don't generate 'Unused variable' warning
            auto value = model->get_stat_string(stat);
            if(!value.empty())
            {
                result = value;
            }
        }
        return result;
    }

    std::vector<intrusive_ptr<ModelEquipment>> SystemEquipment::get_items_with_merge_level(const DataMergeLevel* merge_level)
    {
        std::vector<intrusive_ptr<ModelEquipment>> result;
        for(auto item : this->items)
        {
            if(item->merge_level == merge_level)
            {
                result.push_back(item);
            }
        }
        return result;
    }

    void SystemEquipment::request_merge(ModelEquipment* item_to_merge, const std::vector<intrusive_ptr<ModelEquipment>>& merging_items)
    {
        assert(item_to_merge->merge_level->next);
        for(auto item : merging_items)
        {
            assert(in_list(item, this->items));
            list_remove(this->items, item);
        }
        this->event_inventory_changed.notify();
        item_to_merge->merge_level = item_to_merge->merge_level->next;
    }

    bool SystemEquipment::operator ==(const SystemEquipment& rhs) const
    {
        bool result = this->System::operator ==(rhs);
        result = result && this->slots == rhs.slots;
        result = result && this->items == rhs.items;
        return result;
    }

    bool SystemEquipment::operator !=(const SystemEquipment& rhs) const
    {
        return !(*this == rhs);
    }

    SystemEquipment::SystemEquipment(const SystemEquipment& rhs)
    {
        this->operator=(rhs);
    }

    const SystemEquipment& SystemEquipment::operator =(const SystemEquipment& rhs)
    {
        this->System::operator=(rhs);
        this->slots = rhs.slots;
        this->items = rhs.items;
        this->event_inventory_changed = rhs.event_inventory_changed;
        this->event_slot_changed = rhs.event_slot_changed;
        return *this;
    }

    std::string SystemEquipment::get_type() const
    {
        return SystemEquipment::TYPE;
    }

    void SystemEquipment::serialize_xml(SerializerXml& serializer) const
    {
        System::serialize_xml(serializer);
        serializer.serialize(slots, "slots");
        serializer.serialize(items, "items");
    }

    void SystemEquipment::deserialize_xml(DeserializerXml& deserializer)
    {
        System::deserialize_xml(deserializer);
        deserializer.deserialize(slots, "slots");
        deserializer.deserialize(items, "items");
    }

    void SystemEquipment::serialize_json(SerializerJson& serializer) const
    {
        System::serialize_json(serializer);
        serializer.serialize(slots, "slots");
        serializer.serialize(items, "items");
    }

    void SystemEquipment::deserialize_json(DeserializerJson& deserializer)
    {
        System::deserialize_json(deserializer);
        deserializer.deserialize(slots, "slots");
        deserializer.deserialize(items, "items");
    }

} //namespace mg
