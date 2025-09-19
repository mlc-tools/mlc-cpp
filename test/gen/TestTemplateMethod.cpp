#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BarClassFromBase.h"
#include "BaseClass.h"
#include "FooClassFromBase.h"
#include "TemplateMethod.h"
#include "TestTemplateMethod.h"
#include "tests/ITestTemplateMethod.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestTemplateMethod::TYPE("TestTemplateMethod");

    
    TestTemplateMethod::TestTemplateMethod()
    {
    
    }

    TestTemplateMethod::~TestTemplateMethod(){}


void TestTemplateMethod::test_get()
{

        auto test = make_intrusive<TemplateMethod>();
        test->members["base"] = make_intrusive<BaseClass>();
        test->members["foo"] = make_intrusive<FooClassFromBase>();
        test->members["bar"] = make_intrusive<BarClassFromBase>();

        this->assertNotNull(test->get<BaseClass>("base"), "test->get<BaseClass>'base'");
        this->assertNotNull(test->get<BaseClass>("foo"), "test->get<BaseClass>'foo'");
        this->assertNotNull(test->get<BaseClass>("bar"), "test->get<BaseClass>'bar'");
        this->assertNotNull(test->get<FooClassFromBase>("foo"), "test->get<FooClassFromBase>'foo'");
        this->assertNotNull(test->get<BarClassFromBase>("bar"), "test->get<BarClassFromBase>'bar'");
        this->assertNull(test->get<FooClassFromBase>("base"), "test->get<FooClassFromBase>'base'");
        this->assertNull(test->get<FooClassFromBase>("bar"), "test->get<FooClassFromBase>'bar'");
        this->assertNull(test->get<BarClassFromBase>("base"), "test->get<BarClassFromBase>'base'");
        this->assertNull(test->get<BarClassFromBase>("foo"), "test->get<BarClassFromBase>'foo'");
    
}

void TestTemplateMethod::test_default()
{

        this->assertEqual(Default<int>::value, 0);
        this->assertEqual(Default<unsigned int>::value, 0u);
        this->assertEqual(Default<int64_t>::value, 0ll);
        this->assertEqual(Default<float>::value, 0.f);
        this->assertEqual(Default<bool>::value, false);
        this->assertEqual(Default<std::string>::value, std::string(""));
    
}

bool TestTemplateMethod::operator ==(const TestTemplateMethod& rhs) const
{

bool result = this->ITestTemplateMethod::operator ==(rhs);
return result;
}

bool TestTemplateMethod::operator !=(const TestTemplateMethod& rhs) const
{

return !(*this == rhs);
}

 TestTemplateMethod::TestTemplateMethod(const TestTemplateMethod& rhs)
{

this->operator=(rhs);
}

const TestTemplateMethod& TestTemplateMethod::operator =(const TestTemplateMethod& rhs)
{

this->ITestTemplateMethod::operator=(rhs);
return *this;
}

std::string TestTemplateMethod::get_type() const
{
return TestTemplateMethod::TYPE;
}

void TestTemplateMethod::serialize_xml(SerializerXml& serializer) const
{

}

void TestTemplateMethod::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestTemplateMethod::serialize_json(SerializerJson& serializer) const
{

}

void TestTemplateMethod::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
