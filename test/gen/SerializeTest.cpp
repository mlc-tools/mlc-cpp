#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SerializeTest.h"
#include "data/DataUnit.h"
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string SerializeTest::TYPE("SerializeTest");

    SerializeTest::SerializeTest()
    : list_of_data()
    , _reference_counter(1)
    {

    }

    SerializeTest::~SerializeTest(){}

    void SerializeTest::retain()
    {
        ++this->_reference_counter;
    }

    int SerializeTest::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SerializeTest::operator ==(const SerializeTest& rhs) const
    {
        bool result = true;
        result = result && this->list_of_data == rhs.list_of_data;
        return result;
    }

    bool SerializeTest::operator !=(const SerializeTest& rhs) const
    {

        return !(*this == rhs);
    }

    SerializeTest::SerializeTest(const SerializeTest& rhs)
    {

        this->operator=(rhs);
    }

    const SerializeTest& SerializeTest::operator =(const SerializeTest& rhs)
    {

        this->list_of_data = rhs.list_of_data;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SerializeTest::get_type() const
    {
        return SerializeTest::TYPE;
    }

    void SerializeTest::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(list_of_data, "list_of_data");
    }

    void SerializeTest::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(list_of_data, "list_of_data");
    }

    void SerializeTest::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(list_of_data, "list_of_data");
    }

    void SerializeTest::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(list_of_data, "list_of_data");
    }

} // namespace mg
