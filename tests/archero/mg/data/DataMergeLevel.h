#ifndef __mg_DataMergeLevel_h__
#define __mg_DataMergeLevel_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../VisualItem.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class DataMergeLevel
    {
    public:
        DataMergeLevel();
        ~DataMergeLevel();
        void retain();
        int release();
        bool operator ==(const DataMergeLevel& rhs) const;
        bool operator !=(const DataMergeLevel& rhs) const;
        DataMergeLevel(const DataMergeLevel& rhs);
        const DataMergeLevel& operator =(const DataMergeLevel& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
        VisualItem visual;
        int index;
        const DataMergeLevel* next;
        std::vector<const DataMergeLevel*> require_items_to_merge;
        int count_items_to_merge;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataMergeLevel_h__
