#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "NullishOperator.h"
#include "Point.h"
#include <atomic>
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string NullishOperator::TYPE("NullishOperator");

    NullishOperator::NullishOperator()
    : point_a(nullptr)
    , point_b(nullptr)
    , _reference_counter(1)
    {

    }

    NullishOperator::~NullishOperator(){}

    void NullishOperator::dummy_function()
    {

    }

    void NullishOperator::retain()
    {
        ++this->_reference_counter;
    }

    int NullishOperator::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool NullishOperator::operator ==(const NullishOperator& rhs) const
    {
        bool result = true;
        result = result && ((this->point_a == rhs.point_a) || (this->point_a != nullptr && rhs.point_a != nullptr && *this->point_a == *rhs.point_a));
        result = result && ((this->point_b == rhs.point_b) || (this->point_b != nullptr && rhs.point_b != nullptr && *this->point_b == *rhs.point_b));
        return result;
    }

    bool NullishOperator::operator !=(const NullishOperator& rhs) const
    {

        return !(*this == rhs);
    }

    NullishOperator::NullishOperator(const NullishOperator& rhs)
    {

        this->operator=(rhs);
    }

    const NullishOperator& NullishOperator::operator =(const NullishOperator& rhs)
    {

        this->point_a = rhs.point_a;
        this->point_b = rhs.point_b;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string NullishOperator::get_type() const
    {
        return NullishOperator::TYPE;
    }

    void NullishOperator::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(point_a, "point_a");
        serializer.serialize(point_b, "point_b");
    }

    void NullishOperator::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(point_a, "point_a");
        deserializer.deserialize(point_b, "point_b");
    }

    void NullishOperator::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(point_a, "point_a");
        serializer.serialize(point_b, "point_b");
    }

    void NullishOperator::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(point_a, "point_a");
        deserializer.deserialize(point_b, "point_b");
    }

} // namespace mg
