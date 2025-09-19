#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "TestUser1.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestUser1::TYPE("TestUser1");

    TestUser1::TestUser1()
    : result(true)
    , _reference_counter(1)
    {

    }

    TestUser1::~TestUser1(){}

    void TestUser1::retain()
    {
        ++this->_reference_counter;
    }

    int TestUser1::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool TestUser1::operator ==(const TestUser1& rhs) const
    {
        bool result = true;
        result = result && this->result == rhs.result;
        return result;
    }

    bool TestUser1::operator !=(const TestUser1& rhs) const
    {

        return !(*this == rhs);
    }

    TestUser1::TestUser1(const TestUser1& rhs)
    {

        this->operator=(rhs);
    }

    const TestUser1& TestUser1::operator =(const TestUser1& rhs)
    {

        this->result = rhs.result;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string TestUser1::get_type() const
    {
        return TestUser1::TYPE;
    }

    void TestUser1::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(result, "result", bool(true));
    }

    void TestUser1::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(result, "result", bool(true));
    }

    void TestUser1::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(result, "result", bool(true));
    }

    void TestUser1::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(result, "result", bool(true));
    }

} // namespace mg
