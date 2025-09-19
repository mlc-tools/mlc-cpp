#ifndef __mg_AllTypesChildren_h__
#define __mg_AllTypesChildren_h__

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

    class AllTypesChildren
    {
    public:
        AllTypesChildren();
        ~AllTypesChildren();
        void retain();
        int release();
        bool operator ==(const AllTypesChildren& rhs) const;
        bool operator !=(const AllTypesChildren& rhs) const;
        AllTypesChildren(const AllTypesChildren& rhs);
        const AllTypesChildren& operator =(const AllTypesChildren& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        int value;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_AllTypesChildren_h__
