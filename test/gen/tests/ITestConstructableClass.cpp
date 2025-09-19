#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestConstructableClass.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestConstructableClass::TYPE("ITestConstructableClass");

    
    ITestConstructableClass::ITestConstructableClass()
    {
    
    }

    ITestConstructableClass::~ITestConstructableClass(){}


void ITestConstructableClass::execute()
{
this->test_constructor();
this->test_constructor_with_args();
this->test_constructor_with_args_2();

}

bool ITestConstructableClass::operator ==(const ITestConstructableClass& rhs) const
{

bool result = true;
return result;
}

bool ITestConstructableClass::operator !=(const ITestConstructableClass& rhs) const
{

return !(*this == rhs);
}

 ITestConstructableClass::ITestConstructableClass(const ITestConstructableClass& rhs)
{

this->operator=(rhs);
}

const ITestConstructableClass& ITestConstructableClass::operator =(const ITestConstructableClass& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestConstructableClass::get_type() const
{
return ITestConstructableClass::TYPE;
}

void ITestConstructableClass::serialize_xml(SerializerXml& serializer) const
{

}

void ITestConstructableClass::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestConstructableClass::serialize_json(SerializerJson& serializer) const
{

}

void ITestConstructableClass::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
