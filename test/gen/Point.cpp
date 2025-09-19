#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "Point.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string Point::TYPE("Point");

    Point::Point()
    : x(0)
    , y(0)
    , _reference_counter(1)
    {

    }

    Point::~Point(){}

    Point Point::create(int x, int y)
    {

        Point point;
        point.x = x;
        point.y = y;
        return point;

    }

    void Point::retain()
    {
        ++this->_reference_counter;
    }

    int Point::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Point::operator ==(const Point& rhs) const
    {
        bool result = true;
        result = result && this->x == rhs.x;
        result = result && this->y == rhs.y;
        return result;
    }

    bool Point::operator !=(const Point& rhs) const
    {

        return !(*this == rhs);
    }

    Point::Point(const Point& rhs)
    {

        this->operator=(rhs);
    }

    const Point& Point::operator =(const Point& rhs)
    {

        this->x = rhs.x;
        this->y = rhs.y;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Point::get_type() const
    {
        return Point::TYPE;
    }

    void Point::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(x, "x", int(0));
        serializer.serialize(y, "y", int(0));
    }

    void Point::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(x, "x", int(0));
        deserializer.deserialize(y, "y", int(0));
    }

    void Point::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(x, "x", int(0));
        serializer.serialize(y, "y", int(0));
    }

    void Point::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(x, "x", int(0));
        deserializer.deserialize(y, "y", int(0));
    }

} // namespace mg
