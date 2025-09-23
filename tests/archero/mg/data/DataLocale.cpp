#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataLocale.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataLocale::TYPE("DataLocale");

    DataLocale::DataLocale()
    : name("")
    , value("")
    , _reference_counter(1)
    {

    }

    DataLocale::~DataLocale()
    {
    }

    void DataLocale::retain()
    {
        ++this->_reference_counter;
    }

    int DataLocale::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool DataLocale::operator ==(const DataLocale& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->value == rhs.value;
        return result;
    }

    bool DataLocale::operator !=(const DataLocale& rhs) const
    {
        return !(*this == rhs);
    }

    DataLocale::DataLocale(const DataLocale& rhs)
    {
        this->operator=(rhs);
    }

    const DataLocale& DataLocale::operator =(const DataLocale& rhs)
    {
        this->name = rhs.name;
        this->value = rhs.value;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataLocale::get_type() const
    {
        return DataLocale::TYPE;
    }

    void DataLocale::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(value, "value", std::string(""));
    }

    void DataLocale::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(value, "value", std::string(""));
    }

    void DataLocale::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(value, "value", std::string(""));
    }

    void DataLocale::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(value, "value", std::string(""));
    }

} //namespace mg
