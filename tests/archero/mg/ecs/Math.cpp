#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Math.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Math::TYPE("Math");

    Math::Math()
    : _reference_counter(1)
    {

    }

    Math::~Math()
    {
    }

    float Math::ease_back_in(float t)
    {
        const float c1 = 1.70158f;
        return (c1 + 1) * t * t * t - c1 * t * t;
    }

    float Math::ease_back_out(float t)
    {
        const float c1 = 1.70158f;
        return 1 + (c1 + 1.0f) * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
    }

    float Math::ease_in_out(float t)
    {
        return -(std::cos(M_PI * t) - 1) / 2;
    }

    void Math::retain()
    {
        ++this->_reference_counter;
    }

    int Math::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool Math::operator ==(const Math& rhs) const
    {
        bool result = true;
        return result;
    }

    bool Math::operator !=(const Math& rhs) const
    {
        return !(*this == rhs);
    }

    Math::Math(const Math& rhs)
    {
        this->operator=(rhs);
    }

    const Math& Math::operator =(const Math& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Math::get_type() const
    {
        return Math::TYPE;
    }

    void Math::serialize_xml(SerializerXml& serializer) const
    {

    }

    void Math::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void Math::serialize_json(SerializerJson& serializer) const
    {

    }

    void Math::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
