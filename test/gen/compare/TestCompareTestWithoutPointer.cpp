#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../tests/ITestCompareTestWithoutPointer.h"
#include "CompareTestWithPointer.h"
#include "CompareTestWithoutPointer.h"
#include "TestCompareTestWithoutPointer.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestCompareTestWithoutPointer::TYPE("TestCompareTestWithoutPointer");

    TestCompareTestWithoutPointer::TestCompareTestWithoutPointer()
    {

    }

    TestCompareTestWithoutPointer::~TestCompareTestWithoutPointer(){}

    void TestCompareTestWithoutPointer::test_test()
    {

    }

    void TestCompareTestWithoutPointer::test_operator_copy()
    {

        CompareTestWithoutPointer o1;
        CompareTestWithoutPointer o2;

        o1.id = 1;
        o1.a = 1;
        o1.b = "string";

        o2 = o1;
        this->assertTrue(o1 == o2, "test_operator_copy. o1 == o2");
        this->assertEqual(o1, o2, "test_operator_copy. equal");

    }

    void TestCompareTestWithoutPointer::test_without_pointer_compare()
    {

        CompareTestWithoutPointer o1;
        CompareTestWithoutPointer o2;

        o1.id = 1;
        o1.a = 1;
        o1.b = "string";

        o2.id = 1;
        o2.a = 1;
        o2.b = "string";

        this->assertFalse(o1 != o2, "TestCompare false. !=");
        this->assertFalse(o2 != o1, "TestCompare false. !=");
        this->assertTrue(o1 == o2, "TestCompare. ==");
        this->assertTrue(o2 == o1, "TestCompare. ==");
        this->assertEqual(o1, o2, "TestCompare. equal");
        this->assertEqual(o2, o1, "TestCompare. equal");

        o2.id = 2;
        this->assertTrue(o1 != o2, "TestCompare. !=");
        this->assertTrue(o2 != o1, "TestCompare. !=");
        this->assertFalse(o1 == o2, "TestCompare false. ==");
        this->assertFalse(o2 == o1, "TestCompare false. ==");
        this->assertNotEqual(o1, o2, "TestCompare. not equal 1");
        this->assertNotEqual(o2, o1, "TestCompare. not equal 2");

        o2.b = "other";
        this->assertTrue(o1 != o2, "TestCompare. !=");
        this->assertTrue(o2 != o1, "TestCompare. !=");
        this->assertFalse(o1 == o2, "TestCompare false. ==");
        this->assertFalse(o2 == o1, "TestCompare false. ==");
        this->assertNotEqual(o1, o2, "TestCompare. not equal 3");
        this->assertNotEqual(o2, o1, "TestCompare. not equal 4");

        o2.a = 2;
        o2.b = "string";
        this->assertTrue(o1 != o2, "TestCompare. !=");
        this->assertFalse(o1 == o2, "TestCompare false. ==");
        this->assertNotEqual(o1, o2, "TestCompare. not equal 5");

        o2.a = 2;
        o2.b = "other";
        this->assertTrue(o1 != o2, "TestCompare. !=");
        this->assertFalse(o1 == o2, "TestCompare false. ==");
        this->assertNotEqual(o1, o2, "TestCompare. not equal 6");

    }

    void TestCompareTestWithoutPointer::test_with_pointer_compare()
    {

        CompareTestWithPointer o1;
        CompareTestWithPointer o2;

        this->assertTrue(o1 == o2, "TestCompare with null pointer. ==");
        this->assertTrue(o2 == o1, "TestCompare with null pointer. ==");

        o1.pointer = make_intrusive<CompareTestWithoutPointer>();
        o1.pointer->a = 1;
        o1.pointer->b = "string";

        this->assertTrue(o1 != o2, "TestCompare with null pointer. !=");
        this->assertTrue(o2 != o1, "TestCompare with null pointer. !=");

        o2.pointer = make_intrusive<CompareTestWithoutPointer>();
        o2.pointer->a = 1;
        o2.pointer->b = "string";

        this->assertTrue(o1 == o2, "TestCompare with pointer. ==");
        this->assertEqual(o1, o2, "TestCompare with pointer. equal");

        o2.pointer->a = 2;
        o2.pointer->b = "other";
        this->assertTrue(o1 != o2, "TestCompare with pointer. !=");
        this->assertTrue(o2 != o1, "TestCompare with pointer. !=");
        this->assertFalse(o1 == o2, "TestCompare with pointer false. ==");
        this->assertFalse(o2 == o1, "TestCompare with pointer false. ==");
        this->assertNotEqual(o1, o2, "TestCompare with pointer. not equal");
        this->assertNotEqual(o2, o1, "TestCompare with pointer. not equal");

    }

    bool TestCompareTestWithoutPointer::operator ==(const TestCompareTestWithoutPointer& rhs) const
    {

        bool result = this->ITestCompareTestWithoutPointer::operator ==(rhs);
        return result;
    }

    bool TestCompareTestWithoutPointer::operator !=(const TestCompareTestWithoutPointer& rhs) const
    {

        return !(*this == rhs);
    }

    TestCompareTestWithoutPointer::TestCompareTestWithoutPointer(const TestCompareTestWithoutPointer& rhs)
    {

        this->operator=(rhs);
    }

    const TestCompareTestWithoutPointer& TestCompareTestWithoutPointer::operator =(const TestCompareTestWithoutPointer& rhs)
    {

        this->ITestCompareTestWithoutPointer::operator=(rhs);
        return *this;
    }

    std::string TestCompareTestWithoutPointer::get_type() const
    {
        return TestCompareTestWithoutPointer::TYPE;
    }

    void TestCompareTestWithoutPointer::serialize_xml(SerializerXml& serializer) const
    {

    }

    void TestCompareTestWithoutPointer::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void TestCompareTestWithoutPointer::serialize_json(SerializerJson& serializer) const
    {

    }

    void TestCompareTestWithoutPointer::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
