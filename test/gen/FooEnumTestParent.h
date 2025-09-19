#ifndef __mg_FooEnumTestParent_h__
#define __mg_FooEnumTestParent_h__

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

    class FooEnumTestParent
    {
    public:
        FooEnumTestParent();
        ~FooEnumTestParent();
        void dummy();
        void retain();
        int release();
        bool operator ==(const FooEnumTestParent& rhs) const;
        bool operator !=(const FooEnumTestParent& rhs) const;
        FooEnumTestParent(const FooEnumTestParent& rhs);
        const FooEnumTestParent& operator =(const FooEnumTestParent& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_FooEnumTestParent_h__
