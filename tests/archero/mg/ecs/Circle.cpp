#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Circle.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Circle::TYPE("Circle");

    Circle::Circle(const Vector& center, float radius)
    : center()
    , radius(0.0)
    {
        this->center = center;
        this->radius = radius;
    }

    Circle::~Circle()
    {
    }

    bool Circle::operator ==(const Circle& rhs) const
    {
        bool result = this->PhysicalBody::operator ==(rhs);
        result = result && this->center == rhs.center;
        result = result && this->radius == rhs.radius;
        return result;
    }

    bool Circle::operator !=(const Circle& rhs) const
    {
        return !(*this == rhs);
    }

    Circle::Circle(const Circle& rhs)
    {
        this->operator=(rhs);
    }

    const Circle& Circle::operator =(const Circle& rhs)
    {
        this->PhysicalBody::operator=(rhs);
        this->center = rhs.center;
        this->radius = rhs.radius;
        return *this;
    }

    std::string Circle::get_type() const
    {
        return Circle::TYPE;
    }

    void Circle::serialize_xml(SerializerXml& serializer) const
    {
        PhysicalBody::serialize_xml(serializer);
        serializer.serialize(center, "center");
        serializer.serialize(radius, "radius", float(0.0));
    }

    void Circle::deserialize_xml(DeserializerXml& deserializer)
    {
        PhysicalBody::deserialize_xml(deserializer);
        deserializer.deserialize(center, "center");
        deserializer.deserialize(radius, "radius", float(0.0));
    }

    void Circle::serialize_json(SerializerJson& serializer) const
    {
        PhysicalBody::serialize_json(serializer);
        serializer.serialize(center, "center");
        serializer.serialize(radius, "radius", float(0.0));
    }

    void Circle::deserialize_json(DeserializerJson& deserializer)
    {
        PhysicalBody::deserialize_json(deserializer);
        deserializer.deserialize(center, "center");
        deserializer.deserialize(radius, "radius", float(0.0));
    }

} //namespace mg
