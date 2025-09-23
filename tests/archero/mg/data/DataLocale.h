#ifndef __mg_DataLocale_h__
#define __mg_DataLocale_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class DataLocale
    {
    public:
        DataLocale();
        ~DataLocale();
        void retain();
        int release();
        bool operator ==(const DataLocale& rhs) const;
        bool operator !=(const DataLocale& rhs) const;
        DataLocale(const DataLocale& rhs);
        const DataLocale& operator =(const DataLocale& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
        std::string value;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataLocale_h__
