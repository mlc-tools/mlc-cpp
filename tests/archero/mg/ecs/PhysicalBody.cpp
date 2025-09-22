#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "PhysicalBody.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string PhysicalBody::TYPE("PhysicalBody");

    PhysicalBody::PhysicalBody()
    : _reference_counter(1)
    {

    }

    PhysicalBody::~PhysicalBody()
    {
    }

    void PhysicalBody::retain()
    {
        ++this->_reference_counter;
    }

    int PhysicalBody::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool PhysicalBody::operator ==(const PhysicalBody& rhs) const
    {
        bool result = true;
        return result;
    }

    bool PhysicalBody::operator !=(const PhysicalBody& rhs) const
    {
        return !(*this == rhs);
    }

    PhysicalBody::PhysicalBody(const PhysicalBody& rhs)
    {
        this->operator=(rhs);
    }

    const PhysicalBody& PhysicalBody::operator =(const PhysicalBody& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string PhysicalBody::get_type() const
    {
        return PhysicalBody::TYPE;
    }

    void PhysicalBody::serialize_xml(SerializerXml& serializer) const
    {

    }

    void PhysicalBody::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void PhysicalBody::serialize_json(SerializerJson& serializer) const
    {

    }

    void PhysicalBody::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
