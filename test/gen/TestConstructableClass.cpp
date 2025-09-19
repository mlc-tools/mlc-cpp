#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "ConstructableClass.h"
#include "ConstructableClassWithParameters.h"
#include "TestConstructableClass.h"
#include "tests/ITestConstructableClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestConstructableClass::TYPE("TestConstructableClass");

    TestConstructableClass::TestConstructableClass()
    {

    }

    TestConstructableClass::~TestConstructableClass(){}

    void TestConstructableClass::test_constructor()
    {

        auto cls = make_intrusive<ConstructableClass>();
        this->assertTrue(cls->a == 1);

    }

    void TestConstructableClass::test_constructor_with_args()
    {

        auto cls = make_intrusive<ConstructableClassWithParameters>();
        this->assertTrue(cls->a == 0);
        this->assertTrue(cls->b == 0);

    }

    void TestConstructableClass::test_constructor_with_args_2()
    {

        auto cls = make_intrusive<ConstructableClassWithParameters>(1, 2);
        this->assertTrue(cls->a == 1);
        this->assertTrue(cls->b == 2);

    }

    bool TestConstructableClass::operator ==(const TestConstructableClass& rhs) const
    {

        bool result = this->ITestConstructableClass::operator ==(rhs);
        return result;
    }

    bool TestConstructableClass::operator !=(const TestConstructableClass& rhs) const
    {

        return !(*this == rhs);
    }

    TestConstructableClass::TestConstructableClass(const TestConstructableClass& rhs)
    {

        this->operator=(rhs);
    }

    const TestConstructableClass& TestConstructableClass::operator =(const TestConstructableClass& rhs)
    {

        this->ITestConstructableClass::operator=(rhs);
        return *this;
    }

    std::string TestConstructableClass::get_type() const
    {
        return TestConstructableClass::TYPE;
    }

    void TestConstructableClass::serialize_xml(SerializerXml& serializer) const
    {

    }

    void TestConstructableClass::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void TestConstructableClass::serialize_json(SerializerJson& serializer) const
    {

    }

    void TestConstructableClass::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
