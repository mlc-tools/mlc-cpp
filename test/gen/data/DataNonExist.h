#ifndef __mg_DataNonExist_h__
#define __mg_DataNonExist_h__

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

    class DataNonExist
    {
    public:
        DataNonExist();
        ~DataNonExist();
        void retain();
        int release();
        bool operator ==(const DataNonExist& rhs) const;
        bool operator !=(const DataNonExist& rhs) const;
        DataNonExist(const DataNonExist& rhs);
        const DataNonExist& operator =(const DataNonExist& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_DataNonExist_h__
