#ifndef __mg_DataLevel_h__
#define __mg_DataLevel_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "DataWaveBase.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class DataWaveBase;

    class DataLevel
    {
    public:
        DataLevel();
        ~DataLevel();
        bool is_change_view_on_next_wave() const;
        void retain();
        int release();
        bool operator ==(const DataLevel& rhs) const;
        bool operator !=(const DataLevel& rhs) const;
        DataLevel(const DataLevel& rhs);
        const DataLevel& operator =(const DataLevel& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
        std::vector<std::string> tmx_maps;
        std::vector<intrusive_ptr<DataWaveBase>> waves;
        float enemies_hp_rate;
        float enemies_damage_rate;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataLevel_h__
