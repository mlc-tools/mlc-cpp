#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BBox.h"
#include "Vector.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string BBox::TYPE("BBox");

    BBox::BBox()
    : min()
    , max()
    , _reference_counter(1)
    {

    }

    BBox::~BBox()
    {
    }

    void BBox::update(const std::vector<Vector>& points)
    {
        if(points.size() == 0)
        {
            this->min = Vector::ZERO;
            this->max = Vector::ZERO;
            return;
        }
        this->min = points[0];
        this->max = points[0];
        for(auto& p : points)
        {
            if(p.x < this->min.x) this->min.x = p.x;
            if(p.y < this->min.y) this->min.y = p.y;
            if(p.x > this->max.x) this->max.x = p.x;
            if(p.y > this->max.y) this->max.y = p.y;
        }
    }

    void BBox::retain()
    {
        ++this->_reference_counter;
    }

    int BBox::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool BBox::operator ==(const BBox& rhs) const
    {
        bool result = true;
        result = result && this->min == rhs.min;
        result = result && this->max == rhs.max;
        return result;
    }

    bool BBox::operator !=(const BBox& rhs) const
    {
        return !(*this == rhs);
    }

    BBox::BBox(const BBox& rhs)
    {
        this->operator=(rhs);
    }

    const BBox& BBox::operator =(const BBox& rhs)
    {
        this->min = rhs.min;
        this->max = rhs.max;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string BBox::get_type() const
    {
        return BBox::TYPE;
    }

    void BBox::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(min, "min");
        serializer.serialize(max, "max");
    }

    void BBox::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(min, "min");
        deserializer.deserialize(max, "max");
    }

    void BBox::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(min, "min");
        serializer.serialize(max, "max");
    }

    void BBox::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(min, "min");
        deserializer.deserialize(max, "max");
    }

} //namespace mg
