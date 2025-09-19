#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../OuterClassInnerClass.h"
#include "../tests/ITestOuterClass.h"
#include "OuterClass.h"
#include "TestOuterClass.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestOuterClass::TYPE("TestOuterClass");

    
    TestOuterClass::TestOuterClass()
    {
    
    }

    TestOuterClass::~TestOuterClass(){}


void TestOuterClass::test_func()
{

        OuterClass cls;
        cls.func();
    
}

void TestOuterClass::test_func2()
{

        OuterClassInnerClass inner;
        OuterClass cls;
        cls.func2(inner);
    
}

bool TestOuterClass::operator ==(const TestOuterClass& rhs) const
{

bool result = this->ITestOuterClass::operator ==(rhs);
return result;
}

bool TestOuterClass::operator !=(const TestOuterClass& rhs) const
{

return !(*this == rhs);
}

 TestOuterClass::TestOuterClass(const TestOuterClass& rhs)
{

this->operator=(rhs);
}

const TestOuterClass& TestOuterClass::operator =(const TestOuterClass& rhs)
{

this->ITestOuterClass::operator=(rhs);
return *this;
}

std::string TestOuterClass::get_type() const
{
return TestOuterClass::TYPE;
}

void TestOuterClass::serialize_xml(SerializerXml& serializer) const
{

}

void TestOuterClass::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestOuterClass::serialize_json(SerializerJson& serializer) const
{

}

void TestOuterClass::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
