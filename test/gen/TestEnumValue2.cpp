#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "TestEnum.h"
#include "TestEnumValue2.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestEnumValue2::TYPE("TestEnumValue2");

    
    TestEnumValue2::TestEnumValue2()
    : parameter(0)
{
    
    }

    TestEnumValue2::~TestEnumValue2(){}


bool TestEnumValue2::operator ==(const TestEnumValue2& rhs) const
{

bool result = this->TestEnum::operator ==(rhs);
result = result && this->parameter == rhs.parameter;
return result;
}

bool TestEnumValue2::operator !=(const TestEnumValue2& rhs) const
{

return !(*this == rhs);
}

 TestEnumValue2::TestEnumValue2(const TestEnumValue2& rhs)
{

this->operator=(rhs);
}

const TestEnumValue2& TestEnumValue2::operator =(const TestEnumValue2& rhs)
{

this->TestEnum::operator=(rhs);
this->parameter = rhs.parameter;
return *this;
}

std::string TestEnumValue2::get_type() const
{
return TestEnumValue2::TYPE;
}

void TestEnumValue2::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(parameter, "parameter", int(0));
}

void TestEnumValue2::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(parameter, "parameter", int(0));
}

void TestEnumValue2::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(parameter, "parameter", int(0));
}

void TestEnumValue2::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(parameter, "parameter", int(0));
}

} // namespace mg
