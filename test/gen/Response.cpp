#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "Response.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string Response::TYPE("Response");

    
    Response::Response()
    : _reference_counter(1)
{
    
    }

    Response::~Response(){}


void Response::retain()
{
++this->_reference_counter;
}

int Response::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool Response::operator ==(const Response& rhs) const
{
bool result = true;
return result;
}

bool Response::operator !=(const Response& rhs) const
{

return !(*this == rhs);
}

 Response::Response(const Response& rhs)
{

this->operator=(rhs);
}

const Response& Response::operator =(const Response& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string Response::get_type() const
{
return Response::TYPE;
}

void Response::serialize_xml(SerializerXml& serializer) const
{

}

void Response::deserialize_xml(DeserializerXml& deserializer)
{

}

void Response::serialize_json(SerializerJson& serializer) const
{

}

void Response::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
