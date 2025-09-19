#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "IVisitorRequest.h"
#include "Request.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string Request::TYPE("Request");

    
    Request::Request()
    : _reference_counter(1)
{
    
    }

    Request::~Request(){}


void Request::accept(IVisitorRequest* visitor)
{
visitor->visit(this);
}

void Request::retain()
{
++this->_reference_counter;
}

int Request::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool Request::operator ==(const Request& rhs) const
{
bool result = true;
return result;
}

bool Request::operator !=(const Request& rhs) const
{

return !(*this == rhs);
}

 Request::Request(const Request& rhs)
{

this->operator=(rhs);
}

const Request& Request::operator =(const Request& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string Request::get_type() const
{
return Request::TYPE;
}

void Request::serialize_xml(SerializerXml& serializer) const
{

}

void Request::deserialize_xml(DeserializerXml& deserializer)
{

}

void Request::serialize_json(SerializerJson& serializer) const
{

}

void Request::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
