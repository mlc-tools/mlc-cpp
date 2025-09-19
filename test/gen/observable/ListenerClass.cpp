#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../data/DataUnit.h"
#include "ListenerClass.h"
#include "TestObservableClass.h"
#include <atomic>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ListenerClass::TYPE("ListenerClass");

    ListenerClass::ListenerClass()
    : test(nullptr)
    , _reference_counter(1)
    {

    }

    ListenerClass::~ListenerClass(){}

    void ListenerClass::listen()
    {

        this->test->value += 1;

    }

    void ListenerClass::listen_int(int value)
    {

        this->test->value = value;

    }

    void ListenerClass::listen_int_2(int a, int b)
    {

        this->test->value = a + b;

    }

    void ListenerClass::listen_link(const DataUnit* data)
    {

        this->test->value = 1;

    }

    void ListenerClass::listen_pointer(DataUnit* data)
    {

        this->test->value = 1;

    }

    void ListenerClass::retain()
    {
        ++this->_reference_counter;
    }

    int ListenerClass::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool ListenerClass::operator ==(const ListenerClass& rhs) const
    {
        bool result = true;
        result = result && ((this->test == rhs.test) || (this->test != nullptr && rhs.test != nullptr && *this->test == *rhs.test));
        return result;
    }

    bool ListenerClass::operator !=(const ListenerClass& rhs) const
    {

        return !(*this == rhs);
    }

    ListenerClass::ListenerClass(const ListenerClass& rhs)
    {

        this->operator=(rhs);
    }

    const ListenerClass& ListenerClass::operator =(const ListenerClass& rhs)
    {

        this->test = rhs.test;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ListenerClass::get_type() const
    {
        return ListenerClass::TYPE;
    }

    void ListenerClass::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(test, "test");
    }

    void ListenerClass::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(test, "test");
    }

    void ListenerClass::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(test, "test");
    }

    void ListenerClass::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(test, "test");
    }

} // namespace mg
