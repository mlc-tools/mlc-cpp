#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SideTestBase.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string SideTestBase::TYPE("SideTestBase");

    SideTestBase::SideTestBase()
    : _reference_counter(1)
    {

    }

    SideTestBase::~SideTestBase(){}

    void SideTestBase::retain()
    {
        ++this->_reference_counter;
    }

    int SideTestBase::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SideTestBase::operator ==(const SideTestBase& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SideTestBase::operator !=(const SideTestBase& rhs) const
    {

        return !(*this == rhs);
    }

    SideTestBase::SideTestBase(const SideTestBase& rhs)
    {

        this->operator=(rhs);
    }

    const SideTestBase& SideTestBase::operator =(const SideTestBase& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SideTestBase::get_type() const
    {
        return SideTestBase::TYPE;
    }

    void SideTestBase::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SideTestBase::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SideTestBase::serialize_json(SerializerJson& serializer) const
    {

    }

    void SideTestBase::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
