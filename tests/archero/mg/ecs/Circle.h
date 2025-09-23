#ifndef __mg_Circle_h__
#define __mg_Circle_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "PhysicalBody.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class Circle : public PhysicalBody
    {
    public:
        Circle(const Vector& center=Vector::ZERO, float radius=0);
        virtual ~Circle();
        bool operator ==(const Circle& rhs) const;
        bool operator !=(const Circle& rhs) const;
        Circle(const Circle& rhs);
        const Circle& operator =(const Circle& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        Vector center;
        float radius;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Circle_h__
