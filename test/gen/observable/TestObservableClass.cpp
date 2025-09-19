#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../data/DataUnit.h"
#include "../tests/ITestObservableClass.h"
#include "ListenerClass.h"
#include "ObservableClass.h"
#include "TestObservableClass.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestObservableClass::TYPE("TestObservableClass");

    
    TestObservableClass::TestObservableClass()
    : value(0)
, test()
{
    
    }

    TestObservableClass::~TestObservableClass(){}


void TestObservableClass::test_1()
{

	    this->value = 0;
		this->test.eventTest.add(this, &TestObservableClass::listen);
		this->test.eventTest.notify();
		this->assertTrue(this->value == 1, "test_1 1: value == 1");

		this->test.eventTest.remove(this);
		this->test.eventTest.notify();
		this->assertTrue(this->value == 1, "test_1 2: value == 1");
	
}

void TestObservableClass::test_notify_with_unsubscribe()
{

	    this->value = 0;
		this->test.eventTest.add(this, &TestObservableClass::listen_with_remove);
		this->test.eventTest.notify();
		this->assertTrue(this->value == 1, "test_notify_with_unsubscribe 1");

		this->test.eventTest.notify();
		this->assertTrue(this->value == 1, "test_notify_with_unsubscribe 2");
	
}

void TestObservableClass::test_listen_with_two()
{

	    this->value = 0;

		auto listener = make_intrusive<ListenerClass>();
		listener->test = this;
		this->test.eventTest.add(listener, &ListenerClass::listen);
		this->test.eventTest.notify();
		this->assertTrue(this->value == 1, "test_listen_with_two 1");

		auto listener2 = make_intrusive<ListenerClass>();
		listener2->test = this;
		this->test.eventTest.add(listener2, &ListenerClass::listen);
		this->test.eventTest.notify();
		this->assertTrue(this->value == 3, "test_listen_with_two 2");

        this->test.eventTest.remove(listener);
        this->test.eventTest.remove(listener2);
        this->test.eventTest.notify();
        this->assertTrue(this->value == 3, "test_listen_with_two 3");
	
}

void TestObservableClass::test_with_arg_int()
{

		auto listener = make_intrusive<ListenerClass>();
		listener->test = this;
		this->test.eventTestInt.add(listener, &ListenerClass::listen_int, std::placeholders::_1);
		this->test.eventTestInt.notify(1);
		this->assertTrue(this->value == 1, "test_with_arg_int 1");
		this->test.eventTestInt.notify(10);
		this->assertTrue(this->value == 10, "test_with_arg_int 2");
		this->test.eventTestInt.remove(listener);
	
}

void TestObservableClass::test_with_arg_int_two()
{

		auto listener = make_intrusive<ListenerClass>();
		listener->test = this;
		this->test.eventTestInt2.add(listener, &ListenerClass::listen_int_2, std::placeholders::_1, std::placeholders::_2);
		this->test.eventTestInt2.notify(1, 1);
		this->assertTrue(this->value == 2, "test_with_arg_int_two 1");
		this->test.eventTestInt2.notify(10, 10);
		this->assertTrue(this->value == 20, "test_with_arg_int_two 2");
		this->test.eventTestInt2.remove(listener);
	
}

void TestObservableClass::test_with_data()
{

	    auto data = DataStorage::shared().get<DataUnit>("unitname1");
		auto listener = make_intrusive<ListenerClass>();
		listener->test = this;
		this->test.eventTestDataLink.add(listener, &ListenerClass::listen_link, std::placeholders::_1);

		this->value = 0;
		this->test.eventTestDataLink.notify(data);
		this->assertEqual(this->value, 1, "test_with_data 1");
		this->test.eventTestDataLink.remove(listener);
	
}

void TestObservableClass::test_with_pointer()
{

	    auto unit = make_intrusive<DataUnit>();
		auto listener = make_intrusive<ListenerClass>();
		listener->test = this;
		this->test.eventTestDataPointer.add(listener, &ListenerClass::listen_pointer, std::placeholders::_1);

		this->value = 0;
		this->test.eventTestDataPointer.notify(unit);
		this->assertEqual(this->value, 1, "test_with_pointer");
		this->test.eventTestDataPointer.remove(listener);
	
}

void TestObservableClass::listen()
{

		this->value += 1;
	
}

void TestObservableClass::listen_with_remove()
{

	    this->value += 1;
		this->test.eventTest.remove(this);
	
}

bool TestObservableClass::operator ==(const TestObservableClass& rhs) const
{

bool result = this->ITestObservableClass::operator ==(rhs);
result = result && this->value == rhs.value;
result = result && this->test == rhs.test;
return result;
}

bool TestObservableClass::operator !=(const TestObservableClass& rhs) const
{

return !(*this == rhs);
}

 TestObservableClass::TestObservableClass(const TestObservableClass& rhs)
{

this->operator=(rhs);
}

const TestObservableClass& TestObservableClass::operator =(const TestObservableClass& rhs)
{

this->ITestObservableClass::operator=(rhs);
this->value = rhs.value;
this->test = rhs.test;
return *this;
}

std::string TestObservableClass::get_type() const
{
return TestObservableClass::TYPE;
}

void TestObservableClass::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(value, "value", int(0));
serializer.serialize(test, "test");
}

void TestObservableClass::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(value, "value", int(0));
deserializer.deserialize(test, "test");
}

void TestObservableClass::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(value, "value", int(0));
serializer.serialize(test, "test");
}

void TestObservableClass::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(value, "value", int(0));
deserializer.deserialize(test, "test");
}

} // namespace mg
