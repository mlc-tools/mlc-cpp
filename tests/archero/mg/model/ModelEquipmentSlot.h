#ifndef __mg_ModelEquipmentSlot_h__
#define __mg_ModelEquipmentSlot_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../data/EquipmentSlot.h"
#include "ModelEquipment.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentBase;
    class UnitStat;

    class ModelEquipmentSlot
    {
    public:
        ModelEquipmentSlot();
        ~ModelEquipmentSlot();
        float get_stat(const UnitStat& stat, float base_value);
        std::string get_stat_string(const UnitStat& stat);
        std::vector<intrusive_ptr<ComponentBase>> get_ecs_components();
        void retain();
        int release();
        bool operator ==(const ModelEquipmentSlot& rhs) const;
        bool operator !=(const ModelEquipmentSlot& rhs) const;
        ModelEquipmentSlot(const ModelEquipmentSlot& rhs);
        const ModelEquipmentSlot& operator =(const ModelEquipmentSlot& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        EquipmentSlot slot;
        intrusive_ptr<ModelEquipment> item;
        int upgrade_level;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ModelEquipmentSlot_h__
