#ifndef __mg_SystemEquipment_h__
#define __mg_SystemEquipment_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../Observable.h"
#include "../model/ModelEquipment.h"
#include "../model/System.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class DataEquipment;
    class DataMergeLevel;
    class EquipmentSlot;
    class ModelEquipment;
    class ModelEquipmentSlot;
    class ModelUser;
    class UnitStat;

    class SystemEquipment : public System
    {
    public:
        SystemEquipment();
        virtual ~SystemEquipment();
        virtual void initialize(ModelUser* user) override;
        void initialize_slot(EquipmentSlot slot);
        intrusive_ptr<ModelEquipment> add_item(const DataEquipment* data, const DataMergeLevel* merge_level);
        void equip(ModelEquipment* model);
        void unequip(ModelEquipment* model);
        float get_stat(const UnitStat& stat, float base_value);
        std::string get_stat_string(const UnitStat& stat);
        std::vector<intrusive_ptr<ModelEquipment>> get_items_with_merge_level(const DataMergeLevel* merge_level);
        void request_merge(ModelEquipment* item_to_merge, const std::vector<intrusive_ptr<ModelEquipment>>& merging_items);
        bool operator ==(const SystemEquipment& rhs) const;
        bool operator !=(const SystemEquipment& rhs) const;
        SystemEquipment(const SystemEquipment& rhs);
        const SystemEquipment& operator =(const SystemEquipment& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::map<EquipmentSlot, intrusive_ptr<ModelEquipmentSlot>> slots;
        std::vector<intrusive_ptr<ModelEquipment>> items;
        Observable<void()> event_inventory_changed;
        Observable<void(EquipmentSlot)> event_slot_changed;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_SystemEquipment_h__
