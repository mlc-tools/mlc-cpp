#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestRequestAcceptor.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestRequestAcceptor::TYPE("ITestRequestAcceptor");

    
    ITestRequestAcceptor::ITestRequestAcceptor()
    {
    
    }

    ITestRequestAcceptor::~ITestRequestAcceptor(){}


void ITestRequestAcceptor::execute()
{

}

bool ITestRequestAcceptor::operator ==(const ITestRequestAcceptor& rhs) const
{

bool result = true;
return result;
}

bool ITestRequestAcceptor::operator !=(const ITestRequestAcceptor& rhs) const
{

return !(*this == rhs);
}

 ITestRequestAcceptor::ITestRequestAcceptor(const ITestRequestAcceptor& rhs)
{

this->operator=(rhs);
}

const ITestRequestAcceptor& ITestRequestAcceptor::operator =(const ITestRequestAcceptor& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestRequestAcceptor::get_type() const
{
return ITestRequestAcceptor::TYPE;
}

void ITestRequestAcceptor::serialize_xml(SerializerXml& serializer) const
{

}

void ITestRequestAcceptor::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestRequestAcceptor::serialize_json(SerializerJson& serializer) const
{

}

void ITestRequestAcceptor::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
