#ifndef __mg_ModelEquipment_h__
#define __mg_ModelEquipment_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../data/DataEquipment.h"
#include "../data/DataMergeLevel.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class UnitStat;

    class ModelEquipment
    {
    public:
        ModelEquipment();
        ~ModelEquipment();
        float get_base_value(const UnitStat& stat);
        void retain();
        int release();
        bool operator ==(const ModelEquipment& rhs) const;
        bool operator !=(const ModelEquipment& rhs) const;
        ModelEquipment(const ModelEquipment& rhs);
        const ModelEquipment& operator =(const ModelEquipment& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        const DataEquipment* data;
        const DataMergeLevel* merge_level;
        bool equip;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ModelEquipment_h__
