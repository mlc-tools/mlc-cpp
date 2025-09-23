#ifndef __mg_PhysicalBody_h__
#define __mg_PhysicalBody_h__

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


    class PhysicalBody
    {
    public:
        PhysicalBody();
        virtual ~PhysicalBody();
        void retain();
        int release();
        bool operator ==(const PhysicalBody& rhs) const;
        bool operator !=(const PhysicalBody& rhs) const;
        PhysicalBody(const PhysicalBody& rhs);
        const PhysicalBody& operator =(const PhysicalBody& rhs);
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
} //namespace mg

#endif //#ifndef __mg_PhysicalBody_h__
