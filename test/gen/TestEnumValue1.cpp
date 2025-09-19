#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "TestEnum.h"
#include "TestEnumValue1.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestEnumValue1::TYPE("TestEnumValue1");

    TestEnumValue1::TestEnumValue1()
    : parameter(0)
    {

    }

    TestEnumValue1::~TestEnumValue1(){}

    bool TestEnumValue1::operator ==(const TestEnumValue1& rhs) const
    {

        bool result = this->TestEnum::operator ==(rhs);
        result = result && this->parameter == rhs.parameter;
        return result;
    }

    bool TestEnumValue1::operator !=(const TestEnumValue1& rhs) const
    {

        return !(*this == rhs);
    }

    TestEnumValue1::TestEnumValue1(const TestEnumValue1& rhs)
    {

        this->operator=(rhs);
    }

    const TestEnumValue1& TestEnumValue1::operator =(const TestEnumValue1& rhs)
    {

        this->TestEnum::operator=(rhs);
        this->parameter = rhs.parameter;
        return *this;
    }

    std::string TestEnumValue1::get_type() const
    {
        return TestEnumValue1::TYPE;
    }

    void TestEnumValue1::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(parameter, "parameter", int(0));
    }

    void TestEnumValue1::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(parameter, "parameter", int(0));
    }

    void TestEnumValue1::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(parameter, "parameter", int(0));
    }

    void TestEnumValue1::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(parameter, "parameter", int(0));
    }

} // namespace mg
