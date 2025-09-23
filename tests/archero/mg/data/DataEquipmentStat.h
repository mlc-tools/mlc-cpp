#ifndef __mg_DataEquipmentStat_h__
#define __mg_DataEquipmentStat_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../ecs/ComponentBase.h"
#include "../ecs/Modifier.h"
#include "../ecs/UnitStat.h"
#include "DataMergeLevel.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class DataEquipmentStat
    {
    public:
        DataEquipmentStat();
        ~DataEquipmentStat();
        void retain();
        int release();
        bool operator ==(const DataEquipmentStat& rhs) const;
        bool operator !=(const DataEquipmentStat& rhs) const;
        DataEquipmentStat(const DataEquipmentStat& rhs);
        const DataEquipmentStat& operator =(const DataEquipmentStat& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        const DataMergeLevel* merge_level;
        UnitStat stat;
        Modifier modifier;
        std::string desc;
        intrusive_ptr<ComponentBase> component;
        bool is_shown_in_ui;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataEquipmentStat_h__
