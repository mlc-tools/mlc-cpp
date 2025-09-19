#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "NullishOperator.h"
#include "Point.h"
#include "TestNullishOperator.h"
#include "tests/ITestNullishOperator.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestNullishOperator::TYPE("TestNullishOperator");

    
    TestNullishOperator::TestNullishOperator()
    {
    
    }

    TestNullishOperator::~TestNullishOperator(){}


void TestNullishOperator::test_dummy_function()
{

    
}

void TestNullishOperator::test_nullish_coalescing_operator()
{

        auto test = make_intrusive<NullishOperator>();
        test->point_a = nullptr;
        test->point_b = make_intrusive<Point>();
        test->point_b->x = 10;
        test->point_b->y = 20;

        Point* point_c = (test->point_a != nullptr) ? test->point_a : (test->point_b);

        this->assertNotNull(point_c);
        this->assertEqual(point_c->x, 10);
        this->assertEqual(point_c->y, 20);
    
}

bool TestNullishOperator::operator ==(const TestNullishOperator& rhs) const
{

bool result = this->ITestNullishOperator::operator ==(rhs);
return result;
}

bool TestNullishOperator::operator !=(const TestNullishOperator& rhs) const
{

return !(*this == rhs);
}

 TestNullishOperator::TestNullishOperator(const TestNullishOperator& rhs)
{

this->operator=(rhs);
}

const TestNullishOperator& TestNullishOperator::operator =(const TestNullishOperator& rhs)
{

this->ITestNullishOperator::operator=(rhs);
return *this;
}

std::string TestNullishOperator::get_type() const
{
return TestNullishOperator::TYPE;
}

void TestNullishOperator::serialize_xml(SerializerXml& serializer) const
{

}

void TestNullishOperator::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestNullishOperator::serialize_json(SerializerJson& serializer) const
{

}

void TestNullishOperator::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
