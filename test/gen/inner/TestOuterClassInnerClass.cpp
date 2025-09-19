#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../tests/ITestOuterClassInnerClass.h"
#include "TestOuterClassInnerClass.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestOuterClassInnerClass::TYPE("TestOuterClassInnerClass");

    TestOuterClassInnerClass::TestOuterClassInnerClass()
    {

    }

    TestOuterClassInnerClass::~TestOuterClassInnerClass(){}

    void TestOuterClassInnerClass::test_func()
    {

    }

    bool TestOuterClassInnerClass::operator ==(const TestOuterClassInnerClass& rhs) const
    {

        bool result = this->ITestOuterClassInnerClass::operator ==(rhs);
        return result;
    }

    bool TestOuterClassInnerClass::operator !=(const TestOuterClassInnerClass& rhs) const
    {

        return !(*this == rhs);
    }

    TestOuterClassInnerClass::TestOuterClassInnerClass(const TestOuterClassInnerClass& rhs)
    {

        this->operator=(rhs);
    }

    const TestOuterClassInnerClass& TestOuterClassInnerClass::operator =(const TestOuterClassInnerClass& rhs)
    {

        this->ITestOuterClassInnerClass::operator=(rhs);
        return *this;
    }

    std::string TestOuterClassInnerClass::get_type() const
    {
        return TestOuterClassInnerClass::TYPE;
    }

    void TestOuterClassInnerClass::serialize_xml(SerializerXml& serializer) const
    {

    }

    void TestOuterClassInnerClass::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void TestOuterClassInnerClass::serialize_json(SerializerJson& serializer) const
    {

    }

    void TestOuterClassInnerClass::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
