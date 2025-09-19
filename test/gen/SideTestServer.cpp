#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SideTestServer.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string SideTestServer::TYPE("SideTestServer");

    
    SideTestServer::SideTestServer()
    : value(0)
, _reference_counter(1)
{
    
    }

    SideTestServer::~SideTestServer(){}


void SideTestServer::retain()
{
++this->_reference_counter;
}

int SideTestServer::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool SideTestServer::operator ==(const SideTestServer& rhs) const
{
bool result = true;
result = result && this->value == rhs.value;
return result;
}

bool SideTestServer::operator !=(const SideTestServer& rhs) const
{

return !(*this == rhs);
}

 SideTestServer::SideTestServer(const SideTestServer& rhs)
{

this->operator=(rhs);
}

const SideTestServer& SideTestServer::operator =(const SideTestServer& rhs)
{

this->value = rhs.value;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string SideTestServer::get_type() const
{
return SideTestServer::TYPE;
}

void SideTestServer::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(value, "value", int(0));
}

void SideTestServer::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(value, "value", int(0));
}

void SideTestServer::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(value, "value", int(0));
}

void SideTestServer::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(value, "value", int(0));
}

} // namespace mg
