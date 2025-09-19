#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataAdd.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string DataAdd::TYPE("DataAdd");

    DataAdd::DataAdd()
    : name()
    , _reference_counter(1)
    {

    }

    DataAdd::~DataAdd(){}

    void DataAdd::test()
    {

    }

    void DataAdd::retain()
    {
        ++this->_reference_counter;
    }

    int DataAdd::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataAdd::operator ==(const DataAdd& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        return result;
    }

    bool DataAdd::operator !=(const DataAdd& rhs) const
    {

        return !(*this == rhs);
    }

    DataAdd::DataAdd(const DataAdd& rhs)
    {

        this->operator=(rhs);
    }

    const DataAdd& DataAdd::operator =(const DataAdd& rhs)
    {

        this->name = rhs.name;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataAdd::get_type() const
    {
        return DataAdd::TYPE;
    }

    void DataAdd::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string());
    }

    void DataAdd::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string());
    }

    void DataAdd::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string());
    }

    void DataAdd::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string());
    }

} // namespace mg
