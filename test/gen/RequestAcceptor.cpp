#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "IVisitorRequest.h"
#include "Request.h"
#include "RequestAcceptor.h"
#include "RequestBar.h"
#include "RequestBar2.h"
#include "RequestFoo.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string RequestAcceptor::TYPE("RequestAcceptor");

    
    RequestAcceptor::RequestAcceptor()
    : base(0)
, foo(0)
, bar(0)
, bar2(0)
{
    
    }

    RequestAcceptor::~RequestAcceptor(){}


void RequestAcceptor::visit(Request* ctx)
{

        this->base += 1;
    
}

void RequestAcceptor::visit(RequestAcceptor* ctx)
{

        this->base += 1;
    
}

void RequestAcceptor::visit(RequestFoo* ctx)
{

        this->foo += 1;
    
}

void RequestAcceptor::visit(RequestBar* ctx)
{

        this->bar += 1;
    
}

void RequestAcceptor::visit(RequestBar2* ctx)
{

        this->bar2 += 1;
    
}

bool RequestAcceptor::operator ==(const RequestAcceptor& rhs) const
{

bool result = this->IVisitorRequest::operator ==(rhs);
result = result && this->base == rhs.base;
result = result && this->foo == rhs.foo;
result = result && this->bar == rhs.bar;
result = result && this->bar2 == rhs.bar2;
return result;
}

bool RequestAcceptor::operator !=(const RequestAcceptor& rhs) const
{

return !(*this == rhs);
}

 RequestAcceptor::RequestAcceptor(const RequestAcceptor& rhs)
{

this->operator=(rhs);
}

const RequestAcceptor& RequestAcceptor::operator =(const RequestAcceptor& rhs)
{

this->IVisitorRequest::operator=(rhs);
this->base = rhs.base;
this->foo = rhs.foo;
this->bar = rhs.bar;
this->bar2 = rhs.bar2;
return *this;
}

std::string RequestAcceptor::get_type() const
{
return RequestAcceptor::TYPE;
}

void RequestAcceptor::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(base, "base", int(0));
serializer.serialize(foo, "foo", int(0));
serializer.serialize(bar, "bar", int(0));
serializer.serialize(bar2, "bar2", int(0));
}

void RequestAcceptor::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(base, "base", int(0));
deserializer.deserialize(foo, "foo", int(0));
deserializer.deserialize(bar, "bar", int(0));
deserializer.deserialize(bar2, "bar2", int(0));
}

void RequestAcceptor::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(base, "base", int(0));
serializer.serialize(foo, "foo", int(0));
serializer.serialize(bar, "bar", int(0));
serializer.serialize(bar2, "bar2", int(0));
}

void RequestAcceptor::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(base, "base", int(0));
deserializer.deserialize(foo, "foo", int(0));
deserializer.deserialize(bar, "bar", int(0));
deserializer.deserialize(bar2, "bar2", int(0));
}

} // namespace mg
