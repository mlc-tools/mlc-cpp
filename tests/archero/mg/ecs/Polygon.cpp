#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BBox.h"
#include "Polygon.h"
#include "Vector.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Polygon::TYPE("Polygon");

    Polygon::Polygon()
    : points()
    , bbox()
    , _reference_counter(1)
    {

    }

    Polygon::~Polygon()
    {
    }

    bool Polygon::is_convex_polygon()
    {
        int n = points.size();
        if (n < 3)
        {
            return false;
        }

        float last_cross = 0.0f;
        bool has_sign = false;

        for (int i = 0; i < n; ++i)
        {
            Vector a = points[i];
            Vector b = points[(i + 1) % n];
            Vector c = points[(i + 2) % n];

            Vector edge1 = b - a;
            Vector edge2 = c - b;

            float cross = edge1.x * edge2.y - edge1.y * edge2.x;

            if (std::fabs(cross) < Vector::FLOAT_SMALL)
            {
                continue;
            }

            if (!has_sign)
            {
                last_cross = cross;
                has_sign = true;
            }
            else
            {

                if ((cross > 0 && last_cross < 0) || (cross < 0 && last_cross > 0))
                {
                    return false;
                }
            }
        }

        return true;
    }

    void Polygon::retain()
    {
        ++this->_reference_counter;
    }

    int Polygon::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool Polygon::operator ==(const Polygon& rhs) const
    {
        bool result = true;
        result = result && this->points == rhs.points;
        result = result && this->bbox == rhs.bbox;
        return result;
    }

    bool Polygon::operator !=(const Polygon& rhs) const
    {
        return !(*this == rhs);
    }

    Polygon::Polygon(const Polygon& rhs)
    {
        this->operator=(rhs);
    }

    const Polygon& Polygon::operator =(const Polygon& rhs)
    {
        this->points = rhs.points;
        this->bbox = rhs.bbox;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Polygon::get_type() const
    {
        return Polygon::TYPE;
    }

    void Polygon::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(points, "points");
        serializer.serialize(bbox, "bbox");
    }

    void Polygon::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(points, "points");
        deserializer.deserialize(bbox, "bbox");
    }

    void Polygon::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(points, "points");
        serializer.serialize(bbox, "bbox");
    }

    void Polygon::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(points, "points");
        deserializer.deserialize(bbox, "bbox");
    }

} //namespace mg
