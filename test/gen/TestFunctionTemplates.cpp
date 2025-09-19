#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "FunctionTemplates.h"
#include "TestFunctionTemplates.h"
#include "tests/ITestFunctionTemplates.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestFunctionTemplates::TYPE("TestFunctionTemplates");

    TestFunctionTemplates::TestFunctionTemplates()
    {

    }

    TestFunctionTemplates::~TestFunctionTemplates(){}

    void TestFunctionTemplates::test_add()
    {

        FunctionTemplates f;

        this->assertEqual(f.add(1, 2), 3, "add(1, 2) == 3");
        this->assertEqual(f.add(-1.f, -2.f), -3.f, "add(-1, -2) == -3");

    }

    void TestFunctionTemplates::test_add_strings()
    {

        FunctionTemplates f;
        std::string a = "a";
        std::string b = "b";
        std::string c = "ab";
        this->assertEqual(f.add(a, b), c, "add('a', 'b') == 'ab'");

    }

    void TestFunctionTemplates::test_use_other_class()
    {

        FunctionTemplates f;
        f.use_other_class(0);

    }

    bool TestFunctionTemplates::operator ==(const TestFunctionTemplates& rhs) const
    {

        bool result = this->ITestFunctionTemplates::operator ==(rhs);
        return result;
    }

    bool TestFunctionTemplates::operator !=(const TestFunctionTemplates& rhs) const
    {

        return !(*this == rhs);
    }

    TestFunctionTemplates::TestFunctionTemplates(const TestFunctionTemplates& rhs)
    {

        this->operator=(rhs);
    }

    const TestFunctionTemplates& TestFunctionTemplates::operator =(const TestFunctionTemplates& rhs)
    {

        this->ITestFunctionTemplates::operator=(rhs);
        return *this;
    }

    std::string TestFunctionTemplates::get_type() const
    {
        return TestFunctionTemplates::TYPE;
    }

    void TestFunctionTemplates::serialize_xml(SerializerXml& serializer) const
    {

    }

    void TestFunctionTemplates::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void TestFunctionTemplates::serialize_json(SerializerJson& serializer) const
    {

    }

    void TestFunctionTemplates::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
