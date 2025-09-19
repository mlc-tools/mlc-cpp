#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestCloneObject.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestCloneObject::TYPE("ITestCloneObject");

    
    ITestCloneObject::ITestCloneObject()
    {
    
    }

    ITestCloneObject::~ITestCloneObject(){}


void ITestCloneObject::execute()
{
this->test_foo();
}

bool ITestCloneObject::operator ==(const ITestCloneObject& rhs) const
{

bool result = true;
return result;
}

bool ITestCloneObject::operator !=(const ITestCloneObject& rhs) const
{

return !(*this == rhs);
}

 ITestCloneObject::ITestCloneObject(const ITestCloneObject& rhs)
{

this->operator=(rhs);
}

const ITestCloneObject& ITestCloneObject::operator =(const ITestCloneObject& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestCloneObject::get_type() const
{
return ITestCloneObject::TYPE;
}

void ITestCloneObject::serialize_xml(SerializerXml& serializer) const
{

}

void ITestCloneObject::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestCloneObject::serialize_json(SerializerJson& serializer) const
{

}

void ITestCloneObject::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
