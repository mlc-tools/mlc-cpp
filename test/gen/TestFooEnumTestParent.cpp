#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataStorage.h"
#include "EnumWithIntValue.h"
#include "FooEnum.h"
#include "TestFooEnumTestParent.h"
#include "data/DataUnit.h"
#include "tests/ITestFooEnumTestParent.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestFooEnumTestParent::TYPE("TestFooEnumTestParent");

    
    TestFooEnumTestParent::TestFooEnumTestParent()
    : enumValue()
, enumFoo(FooEnum::foo)
, enumBar(FooEnum::bar)
, enum_int1(EnumWithIntValue::foo)
, enum_int2(EnumWithIntValue::bar)
, enum_int3(EnumWithIntValue::foo_bar)
{
    
    }

    TestFooEnumTestParent::~TestFooEnumTestParent(){}


void TestFooEnumTestParent::test_dummy()
{

    
}

void TestFooEnumTestParent::test_compare()
{

        this->enumValue = FooEnum::foo;

        this->assertTrue(this->enumValue == FooEnum::foo, "enumValue == FooEnum::foo correct");
        this->assertTrue(this->enumValue == "foo", "enumValue == 'foo' correct");
        this->assertTrue("foo" == this->enumValue, "'foo' == enumValue correct");
    
}

void TestFooEnumTestParent::test_initialize()
{

        this->assertTrue(this->enumFoo == FooEnum::foo, "enumFoo initialized as FooEnum::foo correct");
        this->assertTrue(this->enumBar == FooEnum::bar, "enumBar initialized as FooEnum::bar correct");
    
}

void TestFooEnumTestParent::test_enum_with_int_values()
{

        this->assertTrue(this->enum_int1 == EnumWithIntValue::foo);
        this->assertTrue(this->enum_int2 == EnumWithIntValue::bar);
        this->assertTrue(this->enum_int3 == EnumWithIntValue::foo_bar);
        this->assertTrue(this->enum_int1 & EnumWithIntValue::foo_bar);
        this->assertTrue(this->enum_int2 & EnumWithIntValue::foo_bar);
        this->assertTrue(this->enum_int3 & EnumWithIntValue::foo_bar);
        this->assertFalse(this->enum_int1 & EnumWithIntValue::bar);

        auto unit = DataStorage::shared().get<DataUnit>("unitname1");
        this->assertTrue(unit->enum_with_int_value == EnumWithIntValue::foo);
        this->assertTrue(unit->enum_with_int_value & EnumWithIntValue::foo_bar);
        this->assertTrue(unit->enum_with_int_value | EnumWithIntValue::foo_bar);

        
    
}

bool TestFooEnumTestParent::operator ==(const TestFooEnumTestParent& rhs) const
{

bool result = this->ITestFooEnumTestParent::operator ==(rhs);
result = result && this->enumValue == rhs.enumValue;
result = result && this->enumFoo == rhs.enumFoo;
result = result && this->enumBar == rhs.enumBar;
result = result && this->enum_int1 == rhs.enum_int1;
result = result && this->enum_int2 == rhs.enum_int2;
result = result && this->enum_int3 == rhs.enum_int3;
return result;
}

bool TestFooEnumTestParent::operator !=(const TestFooEnumTestParent& rhs) const
{

return !(*this == rhs);
}

 TestFooEnumTestParent::TestFooEnumTestParent(const TestFooEnumTestParent& rhs)
{

this->operator=(rhs);
}

const TestFooEnumTestParent& TestFooEnumTestParent::operator =(const TestFooEnumTestParent& rhs)
{

this->ITestFooEnumTestParent::operator=(rhs);
this->enumValue = rhs.enumValue;
this->enumFoo = rhs.enumFoo;
this->enumBar = rhs.enumBar;
this->enum_int1 = rhs.enum_int1;
this->enum_int2 = rhs.enum_int2;
this->enum_int3 = rhs.enum_int3;
return *this;
}

std::string TestFooEnumTestParent::get_type() const
{
return TestFooEnumTestParent::TYPE;
}

void TestFooEnumTestParent::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(enumValue, "enumValue");
serializer.serialize(enumFoo, "enumFoo");
serializer.serialize(enumBar, "enumBar");
serializer.serialize(enum_int1, "enum_int1");
serializer.serialize(enum_int2, "enum_int2");
serializer.serialize(enum_int3, "enum_int3");
}

void TestFooEnumTestParent::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(enumValue, "enumValue");
deserializer.deserialize(enumFoo, "enumFoo");
deserializer.deserialize(enumBar, "enumBar");
deserializer.deserialize(enum_int1, "enum_int1");
deserializer.deserialize(enum_int2, "enum_int2");
deserializer.deserialize(enum_int3, "enum_int3");
}

void TestFooEnumTestParent::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(enumValue, "enumValue");
serializer.serialize(enumFoo, "enumFoo");
serializer.serialize(enumBar, "enumBar");
serializer.serialize(enum_int1, "enum_int1");
serializer.serialize(enum_int2, "enum_int2");
serializer.serialize(enum_int3, "enum_int3");
}

void TestFooEnumTestParent::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(enumValue, "enumValue");
deserializer.deserialize(enumFoo, "enumFoo");
deserializer.deserialize(enumBar, "enumBar");
deserializer.deserialize(enum_int1, "enum_int1");
deserializer.deserialize(enum_int2, "enum_int2");
deserializer.deserialize(enum_int3, "enum_int3");
}

} // namespace mg
