#ifndef __mg_BaseClass_h__
#define __mg_BaseClass_h__

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

    class BaseClass
    {
    public:
        BaseClass();
        ~BaseClass();
        void retain();
        int release();
        bool operator ==(const BaseClass& rhs) const;
        bool operator !=(const BaseClass& rhs) const;
        BaseClass(const BaseClass& rhs);
        const BaseClass& operator =(const BaseClass& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_BaseClass_h__
