#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestOuterClassInnerClass.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestOuterClassInnerClass::TYPE("ITestOuterClassInnerClass");

    
    ITestOuterClassInnerClass::ITestOuterClassInnerClass()
    {
    
    }

    ITestOuterClassInnerClass::~ITestOuterClassInnerClass(){}


void ITestOuterClassInnerClass::execute()
{
this->test_func();
}

bool ITestOuterClassInnerClass::operator ==(const ITestOuterClassInnerClass& rhs) const
{

bool result = true;
return result;
}

bool ITestOuterClassInnerClass::operator !=(const ITestOuterClassInnerClass& rhs) const
{

return !(*this == rhs);
}

 ITestOuterClassInnerClass::ITestOuterClassInnerClass(const ITestOuterClassInnerClass& rhs)
{

this->operator=(rhs);
}

const ITestOuterClassInnerClass& ITestOuterClassInnerClass::operator =(const ITestOuterClassInnerClass& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestOuterClassInnerClass::get_type() const
{
return ITestOuterClassInnerClass::TYPE;
}

void ITestOuterClassInnerClass::serialize_xml(SerializerXml& serializer) const
{

}

void ITestOuterClassInnerClass::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestOuterClassInnerClass::serialize_json(SerializerJson& serializer) const
{

}

void ITestOuterClassInnerClass::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
