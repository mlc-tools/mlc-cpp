#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "AbstractClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string AbstractClass::TYPE("AbstractClass");

    AbstractClass::AbstractClass()
    : _reference_counter(1)
    {

    }

    AbstractClass::~AbstractClass(){}

    void AbstractClass::retain()
    {
        ++this->_reference_counter;
    }

    int AbstractClass::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool AbstractClass::operator ==(const AbstractClass& rhs) const
    {
        bool result = true;
        return result;
    }

    bool AbstractClass::operator !=(const AbstractClass& rhs) const
    {

        return !(*this == rhs);
    }

    AbstractClass::AbstractClass(const AbstractClass& rhs)
    {

        this->operator=(rhs);
    }

    const AbstractClass& AbstractClass::operator =(const AbstractClass& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string AbstractClass::get_type() const
    {
        return AbstractClass::TYPE;
    }

    void AbstractClass::serialize_xml(SerializerXml& serializer) const
    {

    }

    void AbstractClass::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void AbstractClass::serialize_json(SerializerJson& serializer) const
    {

    }

    void AbstractClass::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
