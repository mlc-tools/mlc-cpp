#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "TestClassMissingTestMethod.h"
#include "tests/ITestClassMissingTestMethod.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestClassMissingTestMethod::TYPE("TestClassMissingTestMethod");

    
    TestClassMissingTestMethod::TestClassMissingTestMethod()
    {
    
    }

    TestClassMissingTestMethod::~TestClassMissingTestMethod(){}


void TestClassMissingTestMethod::test_foo()
{

}

bool TestClassMissingTestMethod::operator ==(const TestClassMissingTestMethod& rhs) const
{

bool result = this->ITestClassMissingTestMethod::operator ==(rhs);
return result;
}

bool TestClassMissingTestMethod::operator !=(const TestClassMissingTestMethod& rhs) const
{

return !(*this == rhs);
}

 TestClassMissingTestMethod::TestClassMissingTestMethod(const TestClassMissingTestMethod& rhs)
{

this->operator=(rhs);
}

const TestClassMissingTestMethod& TestClassMissingTestMethod::operator =(const TestClassMissingTestMethod& rhs)
{

this->ITestClassMissingTestMethod::operator=(rhs);
return *this;
}

std::string TestClassMissingTestMethod::get_type() const
{
return TestClassMissingTestMethod::TYPE;
}

void TestClassMissingTestMethod::serialize_xml(SerializerXml& serializer) const
{

}

void TestClassMissingTestMethod::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestClassMissingTestMethod::serialize_json(SerializerJson& serializer) const
{

}

void TestClassMissingTestMethod::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
