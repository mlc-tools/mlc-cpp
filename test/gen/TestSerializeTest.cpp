#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SerializeTest.h"
#include "TestSerializeTest.h"
#include "data/DataUnit.h"
#include "tests/ITestSerializeTest.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestSerializeTest::TYPE("TestSerializeTest");

    TestSerializeTest::TestSerializeTest()
    {

    }

    TestSerializeTest::~TestSerializeTest(){}

    void TestSerializeTest::test_serialize_python()
    {

        auto obj = make_intrusive<SerializeTest>();
        auto data = make_intrusive<DataUnit>();
        data->name = "test_unit";
        list_push(obj->list_of_data, data);
        auto obj2 = clone_object<SerializeTest>(obj);
        this->assertNotNull(obj2);
        this->assertEqual(list_size(obj2->list_of_data), 1);
        this->assertNotNull(obj2->list_of_data.at(0));
        this->assertEqual(obj2->list_of_data.at(0)->name, data->name);

    }

    bool TestSerializeTest::operator ==(const TestSerializeTest& rhs) const
    {

        bool result = this->ITestSerializeTest::operator ==(rhs);
        return result;
    }

    bool TestSerializeTest::operator !=(const TestSerializeTest& rhs) const
    {

        return !(*this == rhs);
    }

    TestSerializeTest::TestSerializeTest(const TestSerializeTest& rhs)
    {

        this->operator=(rhs);
    }

    const TestSerializeTest& TestSerializeTest::operator =(const TestSerializeTest& rhs)
    {

        this->ITestSerializeTest::operator=(rhs);
        return *this;
    }

    std::string TestSerializeTest::get_type() const
    {
        return TestSerializeTest::TYPE;
    }

    void TestSerializeTest::serialize_xml(SerializerXml& serializer) const
    {

    }

    void TestSerializeTest::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void TestSerializeTest::serialize_json(SerializerJson& serializer) const
    {

    }

    void TestSerializeTest::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
