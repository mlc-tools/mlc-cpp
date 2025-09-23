#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "PhysicalBody.h"
#include "Ray.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Ray::TYPE("Ray");

    Ray::Ray(const Vector& origin, const Vector& direction)
    : origin()
    , direction()
    {
        this->origin = origin;
        this->direction = direction;
    }

    Ray::~Ray()
    {
    }

    bool Ray::operator ==(const Ray& rhs) const
    {
        bool result = this->PhysicalBody::operator ==(rhs);
        result = result && this->origin == rhs.origin;
        result = result && this->direction == rhs.direction;
        return result;
    }

    bool Ray::operator !=(const Ray& rhs) const
    {
        return !(*this == rhs);
    }

    Ray::Ray(const Ray& rhs)
    {
        this->operator=(rhs);
    }

    const Ray& Ray::operator =(const Ray& rhs)
    {
        this->PhysicalBody::operator=(rhs);
        this->origin = rhs.origin;
        this->direction = rhs.direction;
        return *this;
    }

    std::string Ray::get_type() const
    {
        return Ray::TYPE;
    }

    void Ray::serialize_xml(SerializerXml& serializer) const
    {
        PhysicalBody::serialize_xml(serializer);
        serializer.serialize(origin, "origin");
        serializer.serialize(direction, "direction");
    }

    void Ray::deserialize_xml(DeserializerXml& deserializer)
    {
        PhysicalBody::deserialize_xml(deserializer);
        deserializer.deserialize(origin, "origin");
        deserializer.deserialize(direction, "direction");
    }

    void Ray::serialize_json(SerializerJson& serializer) const
    {
        PhysicalBody::serialize_json(serializer);
        serializer.serialize(origin, "origin");
        serializer.serialize(direction, "direction");
    }

    void Ray::deserialize_json(DeserializerJson& deserializer)
    {
        PhysicalBody::deserialize_json(deserializer);
        deserializer.deserialize(origin, "origin");
        deserializer.deserialize(direction, "direction");
    }

} //namespace mg
