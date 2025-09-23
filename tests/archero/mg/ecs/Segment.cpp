#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "PhysicalBody.h"
#include "Segment.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Segment::TYPE("Segment");

    Segment::Segment(const Vector& start, const Vector& end)
    : start()
    , end()
    {
        this->start = start;
        this->end = end;
    }

    Segment::~Segment()
    {
    }

    bool Segment::operator ==(const Segment& rhs) const
    {
        bool result = this->PhysicalBody::operator ==(rhs);
        result = result && this->start == rhs.start;
        result = result && this->end == rhs.end;
        return result;
    }

    bool Segment::operator !=(const Segment& rhs) const
    {
        return !(*this == rhs);
    }

    Segment::Segment(const Segment& rhs)
    {
        this->operator=(rhs);
    }

    const Segment& Segment::operator =(const Segment& rhs)
    {
        this->PhysicalBody::operator=(rhs);
        this->start = rhs.start;
        this->end = rhs.end;
        return *this;
    }

    std::string Segment::get_type() const
    {
        return Segment::TYPE;
    }

    void Segment::serialize_xml(SerializerXml& serializer) const
    {
        PhysicalBody::serialize_xml(serializer);
        serializer.serialize(start, "start");
        serializer.serialize(end, "end");
    }

    void Segment::deserialize_xml(DeserializerXml& deserializer)
    {
        PhysicalBody::deserialize_xml(deserializer);
        deserializer.deserialize(start, "start");
        deserializer.deserialize(end, "end");
    }

    void Segment::serialize_json(SerializerJson& serializer) const
    {
        PhysicalBody::serialize_json(serializer);
        serializer.serialize(start, "start");
        serializer.serialize(end, "end");
    }

    void Segment::deserialize_json(DeserializerJson& deserializer)
    {
        PhysicalBody::deserialize_json(deserializer);
        deserializer.deserialize(start, "start");
        deserializer.deserialize(end, "end");
    }

} //namespace mg
