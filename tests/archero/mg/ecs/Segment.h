#ifndef __mg_Segment_h__
#define __mg_Segment_h__

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
    class Vector;

    class Segment : public PhysicalBody
    {
    public:
        Segment(const Vector& start=Vector::ZERO, const Vector& end=Vector::ZERO);
        virtual ~Segment();
        bool operator ==(const Segment& rhs) const;
        bool operator !=(const Segment& rhs) const;
        Segment(const Segment& rhs);
        const Segment& operator =(const Segment& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        Vector start;
        Vector end;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Segment_h__
