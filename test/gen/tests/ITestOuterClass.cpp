#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestOuterClass.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestOuterClass::TYPE("ITestOuterClass");

    
    ITestOuterClass::ITestOuterClass()
    {
    
    }

    ITestOuterClass::~ITestOuterClass(){}


void ITestOuterClass::execute()
{
this->test_func();this->test_func2();
}

bool ITestOuterClass::operator ==(const ITestOuterClass& rhs) const
{

bool result = true;
return result;
}

bool ITestOuterClass::operator !=(const ITestOuterClass& rhs) const
{

return !(*this == rhs);
}

 ITestOuterClass::ITestOuterClass(const ITestOuterClass& rhs)
{

this->operator=(rhs);
}

const ITestOuterClass& ITestOuterClass::operator =(const ITestOuterClass& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestOuterClass::get_type() const
{
return ITestOuterClass::TYPE;
}

void ITestOuterClass::serialize_xml(SerializerXml& serializer) const
{

}

void ITestOuterClass::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestOuterClass::serialize_json(SerializerJson& serializer) const
{

}

void ITestOuterClass::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
