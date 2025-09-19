#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SideTestCommon.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string SideTestCommon::TYPE("SideTestCommon");

    SideTestCommon::SideTestCommon()
    : client_value("client")
    , _reference_counter(1)
    {

    }

    SideTestCommon::~SideTestCommon(){}

    std::string SideTestCommon::get_value()
    {

        return this->client_value;

    }

    void SideTestCommon::retain()
    {
        ++this->_reference_counter;
    }

    int SideTestCommon::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SideTestCommon::operator ==(const SideTestCommon& rhs) const
    {
        bool result = true;
        result = result && this->client_value == rhs.client_value;
        return result;
    }

    bool SideTestCommon::operator !=(const SideTestCommon& rhs) const
    {

        return !(*this == rhs);
    }

    SideTestCommon::SideTestCommon(const SideTestCommon& rhs)
    {

        this->operator=(rhs);
    }

    const SideTestCommon& SideTestCommon::operator =(const SideTestCommon& rhs)
    {

        this->client_value = rhs.client_value;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SideTestCommon::get_type() const
    {
        return SideTestCommon::TYPE;
    }

    void SideTestCommon::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(client_value, "client_value", std::string("client"));
    }

    void SideTestCommon::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(client_value, "client_value", std::string("client"));
    }

    void SideTestCommon::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(client_value, "client_value", std::string("client"));
    }

    void SideTestCommon::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(client_value, "client_value", std::string("client"));
    }

} // namespace mg
