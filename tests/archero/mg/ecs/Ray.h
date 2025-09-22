#ifndef __mg_Ray_h__
#define __mg_Ray_h__

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

    class Ray : public PhysicalBody
    {
    public:
        Ray(const Vector& origin=Vector::ZERO, const Vector& direction=Vector::ZERO);
        virtual ~Ray();
        bool operator ==(const Ray& rhs) const;
        bool operator !=(const Ray& rhs) const;
        Ray(const Ray& rhs);
        const Ray& operator =(const Ray& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        Vector origin;
        Vector direction;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Ray_h__
