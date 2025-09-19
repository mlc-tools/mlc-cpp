#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestClassMissingTestMethod.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestClassMissingTestMethod::TYPE("ITestClassMissingTestMethod");

    
    ITestClassMissingTestMethod::ITestClassMissingTestMethod()
    {
    
    }

    ITestClassMissingTestMethod::~ITestClassMissingTestMethod(){}


void ITestClassMissingTestMethod::test_bar()
{

}

void ITestClassMissingTestMethod::execute()
{
this->test_foo();this->test_bar();
}

bool ITestClassMissingTestMethod::operator ==(const ITestClassMissingTestMethod& rhs) const
{

bool result = true;
return result;
}

bool ITestClassMissingTestMethod::operator !=(const ITestClassMissingTestMethod& rhs) const
{

return !(*this == rhs);
}

 ITestClassMissingTestMethod::ITestClassMissingTestMethod(const ITestClassMissingTestMethod& rhs)
{

this->operator=(rhs);
}

const ITestClassMissingTestMethod& ITestClassMissingTestMethod::operator =(const ITestClassMissingTestMethod& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestClassMissingTestMethod::get_type() const
{
return ITestClassMissingTestMethod::TYPE;
}

void ITestClassMissingTestMethod::serialize_xml(SerializerXml& serializer) const
{

}

void ITestClassMissingTestMethod::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestClassMissingTestMethod::serialize_json(SerializerJson& serializer) const
{

}

void ITestClassMissingTestMethod::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
