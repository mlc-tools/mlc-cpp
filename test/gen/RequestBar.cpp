#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "IVisitorRequest.h"
#include "Request.h"
#include "RequestBar.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string RequestBar::TYPE("RequestBar");

    
    RequestBar::RequestBar()
    {
    
    }

    RequestBar::~RequestBar(){}


void RequestBar::accept(IVisitorRequest* visitor)
{
visitor->visit(this);
}

bool RequestBar::operator ==(const RequestBar& rhs) const
{

bool result = this->Request::operator ==(rhs);
return result;
}

bool RequestBar::operator !=(const RequestBar& rhs) const
{

return !(*this == rhs);
}

 RequestBar::RequestBar(const RequestBar& rhs)
{

this->operator=(rhs);
}

const RequestBar& RequestBar::operator =(const RequestBar& rhs)
{

this->Request::operator=(rhs);
return *this;
}

std::string RequestBar::get_type() const
{
return RequestBar::TYPE;
}

void RequestBar::serialize_xml(SerializerXml& serializer) const
{

}

void RequestBar::deserialize_xml(DeserializerXml& deserializer)
{

}

void RequestBar::serialize_json(SerializerJson& serializer) const
{

}

void RequestBar::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
