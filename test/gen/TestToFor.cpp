#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "TestEnum.h"
#include "TestToFor.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestToFor::TYPE("TestToFor");

    TestToFor::TestToFor()
    : some_count(0)
    {

    }

    TestToFor::~TestToFor(){}

    bool TestToFor::operator ==(const TestToFor& rhs) const
    {

        bool result = this->TestEnum::operator ==(rhs);
        result = result && this->some_count == rhs.some_count;
        return result;
    }

    bool TestToFor::operator !=(const TestToFor& rhs) const
    {

        return !(*this == rhs);
    }

    TestToFor::TestToFor(const TestToFor& rhs)
    {

        this->operator=(rhs);
    }

    const TestToFor& TestToFor::operator =(const TestToFor& rhs)
    {

        this->TestEnum::operator=(rhs);
        this->some_count = rhs.some_count;
        return *this;
    }

    std::string TestToFor::get_type() const
    {
        return TestToFor::TYPE;
    }

    void TestToFor::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(some_count, "some_count", int(0));
    }

    void TestToFor::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(some_count, "some_count", int(0));
    }

    void TestToFor::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(some_count, "some_count", int(0));
    }

    void TestToFor::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(some_count, "some_count", int(0));
    }

} // namespace mg
