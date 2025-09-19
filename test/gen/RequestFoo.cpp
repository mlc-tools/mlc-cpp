#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "IVisitorRequest.h"
#include "Request.h"
#include "RequestFoo.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string RequestFoo::TYPE("RequestFoo");

    
    RequestFoo::RequestFoo()
    {
    
    }

    RequestFoo::~RequestFoo(){}


void RequestFoo::accept(IVisitorRequest* visitor)
{
visitor->visit(this);
}

bool RequestFoo::operator ==(const RequestFoo& rhs) const
{

bool result = this->Request::operator ==(rhs);
return result;
}

bool RequestFoo::operator !=(const RequestFoo& rhs) const
{

return !(*this == rhs);
}

 RequestFoo::RequestFoo(const RequestFoo& rhs)
{

this->operator=(rhs);
}

const RequestFoo& RequestFoo::operator =(const RequestFoo& rhs)
{

this->Request::operator=(rhs);
return *this;
}

std::string RequestFoo::get_type() const
{
return RequestFoo::TYPE;
}

void RequestFoo::serialize_xml(SerializerXml& serializer) const
{

}

void RequestFoo::deserialize_xml(DeserializerXml& deserializer)
{

}

void RequestFoo::serialize_json(SerializerJson& serializer) const
{

}

void RequestFoo::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
