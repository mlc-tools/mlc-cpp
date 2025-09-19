#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "TestStaticPointer.h"
#include "tests/ITestStaticPointer.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestStaticPointer::TYPE("TestStaticPointer");

    
    TestStaticPointer::TestStaticPointer()
    {
    
    }

    TestStaticPointer::~TestStaticPointer(){}


void TestStaticPointer::test_foo()
{

        this->assertTrue(true);
    
}

bool TestStaticPointer::operator ==(const TestStaticPointer& rhs) const
{

bool result = this->ITestStaticPointer::operator ==(rhs);
return result;
}

bool TestStaticPointer::operator !=(const TestStaticPointer& rhs) const
{

return !(*this == rhs);
}

 TestStaticPointer::TestStaticPointer(const TestStaticPointer& rhs)
{

this->operator=(rhs);
}

const TestStaticPointer& TestStaticPointer::operator =(const TestStaticPointer& rhs)
{

this->ITestStaticPointer::operator=(rhs);
return *this;
}

std::string TestStaticPointer::get_type() const
{
return TestStaticPointer::TYPE;
}

void TestStaticPointer::serialize_xml(SerializerXml& serializer) const
{

}

void TestStaticPointer::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestStaticPointer::serialize_json(SerializerJson& serializer) const
{

}

void TestStaticPointer::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
