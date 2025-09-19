#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BaseClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string BaseClass::TYPE("BaseClass");

    BaseClass::BaseClass()
    : _reference_counter(1)
    {

    }

    BaseClass::~BaseClass(){}

    void BaseClass::retain()
    {
        ++this->_reference_counter;
    }

    int BaseClass::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool BaseClass::operator ==(const BaseClass& rhs) const
    {
        bool result = true;
        return result;
    }

    bool BaseClass::operator !=(const BaseClass& rhs) const
    {

        return !(*this == rhs);
    }

    BaseClass::BaseClass(const BaseClass& rhs)
    {

        this->operator=(rhs);
    }

    const BaseClass& BaseClass::operator =(const BaseClass& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string BaseClass::get_type() const
    {
        return BaseClass::TYPE;
    }

    void BaseClass::serialize_xml(SerializerXml& serializer) const
    {

    }

    void BaseClass::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void BaseClass::serialize_json(SerializerJson& serializer) const
    {

    }

    void BaseClass::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
