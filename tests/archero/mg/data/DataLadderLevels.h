#ifndef __mg_DataLadderLevels_h__
#define __mg_DataLadderLevels_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class DataLadderLevels
    {
    public:
        DataLadderLevels();
        ~DataLadderLevels();
        float get_value(int level) const;
        int get_level(int exp) const;
        float get_progress_to_next_level(int exp) const;
        int get_progress_to_next_level_int(int exp) const;
        int get_count_to_next_level_int(int exp) const;
        void retain();
        int release();
        bool operator ==(const DataLadderLevels& rhs) const;
        bool operator !=(const DataLadderLevels& rhs) const;
        DataLadderLevels(const DataLadderLevels& rhs);
        const DataLadderLevels& operator =(const DataLadderLevels& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
        std::vector<float> values;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataLadderLevels_h__
