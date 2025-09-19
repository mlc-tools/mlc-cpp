#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "TestTranslatesModel.h"
#include "data/DataUnit.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestTranslatesModel::TYPE("TestTranslatesModel");

    TestTranslatesModel::TestTranslatesModel()
    : data()
    , _reference_counter(1)
    {

    }

    TestTranslatesModel::~TestTranslatesModel(){}

    void TestTranslatesModel::retain()
    {
        ++this->_reference_counter;
    }

    int TestTranslatesModel::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool TestTranslatesModel::operator ==(const TestTranslatesModel& rhs) const
    {
        bool result = true;
        result = result && ((this->data == rhs.data) || (this->data != nullptr && rhs.data != nullptr && *this->data == *rhs.data));
        return result;
    }

    bool TestTranslatesModel::operator !=(const TestTranslatesModel& rhs) const
    {

        return !(*this == rhs);
    }

    TestTranslatesModel::TestTranslatesModel(const TestTranslatesModel& rhs)
    {

        this->operator=(rhs);
    }

    const TestTranslatesModel& TestTranslatesModel::operator =(const TestTranslatesModel& rhs)
    {

        this->data = rhs.data;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string TestTranslatesModel::get_type() const
    {
        return TestTranslatesModel::TYPE;
    }

    void TestTranslatesModel::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(data, "data");
    }

    void TestTranslatesModel::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(data, "data");
    }

    void TestTranslatesModel::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(data, "data");
    }

    void TestTranslatesModel::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(data, "data");
    }

} // namespace mg
