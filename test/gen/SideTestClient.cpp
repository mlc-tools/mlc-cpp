#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SideTestClient.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string SideTestClient::TYPE("SideTestClient");

    SideTestClient::SideTestClient()
    : value(0)
    , _reference_counter(1)
    {

    }

    SideTestClient::~SideTestClient(){}

    void SideTestClient::retain()
    {
        ++this->_reference_counter;
    }

    int SideTestClient::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SideTestClient::operator ==(const SideTestClient& rhs) const
    {
        bool result = true;
        result = result && this->value == rhs.value;
        return result;
    }

    bool SideTestClient::operator !=(const SideTestClient& rhs) const
    {

        return !(*this == rhs);
    }

    SideTestClient::SideTestClient(const SideTestClient& rhs)
    {

        this->operator=(rhs);
    }

    const SideTestClient& SideTestClient::operator =(const SideTestClient& rhs)
    {

        this->value = rhs.value;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SideTestClient::get_type() const
    {
        return SideTestClient::TYPE;
    }

    void SideTestClient::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(value, "value", int(0));
    }

    void SideTestClient::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(value, "value", int(0));
    }

    void SideTestClient::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(value, "value", int(0));
    }

    void SideTestClient::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(value, "value", int(0));
    }

} // namespace mg
