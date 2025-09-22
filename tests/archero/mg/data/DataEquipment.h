#ifndef __mg_DataEquipment_h__
#define __mg_DataEquipment_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../VisualItem.h"
#include "../ecs/UnitStat.h"
#include "DataEquipmentStat.h"
#include "EquipmentSlot.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class DataEquipmentStat;
    class UnitStat;

    class DataEquipment
    {
    public:
        DataEquipment();
        ~DataEquipment();
        void retain();
        int release();
        bool operator ==(const DataEquipment& rhs) const;
        bool operator !=(const DataEquipment& rhs) const;
        DataEquipment(const DataEquipment& rhs);
        const DataEquipment& operator =(const DataEquipment& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        VisualItem visual;
        std::string name;
        std::map<UnitStat, float> stats;
        UnitStat main_stat;
        EquipmentSlot slot;
        std::vector<DataEquipmentStat> merge_stats;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataEquipment_h__
