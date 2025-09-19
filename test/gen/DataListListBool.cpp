#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataListListBool.h"
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string DataListListBool::TYPE("DataListListBool");

    DataListListBool::DataListListBool()
    : name()
    , list_list_bool()
    , _reference_counter(1)
    {

    }

    DataListListBool::~DataListListBool(){}

    void DataListListBool::foo()
    {

    }

    void DataListListBool::retain()
    {
        ++this->_reference_counter;
    }

    int DataListListBool::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataListListBool::operator ==(const DataListListBool& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->list_list_bool == rhs.list_list_bool;
        return result;
    }

    bool DataListListBool::operator !=(const DataListListBool& rhs) const
    {

        return !(*this == rhs);
    }

    DataListListBool::DataListListBool(const DataListListBool& rhs)
    {

        this->operator=(rhs);
    }

    const DataListListBool& DataListListBool::operator =(const DataListListBool& rhs)
    {

        this->name = rhs.name;
        this->list_list_bool = rhs.list_list_bool;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataListListBool::get_type() const
    {
        return DataListListBool::TYPE;
    }

    void DataListListBool::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string());
        serializer.serialize(list_list_bool, "list_list_bool");
    }

    void DataListListBool::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string());
        deserializer.deserialize(list_list_bool, "list_list_bool");
    }

    void DataListListBool::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string());
        serializer.serialize(list_list_bool, "list_list_bool");
    }

    void DataListListBool::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string());
        deserializer.deserialize(list_list_bool, "list_list_bool");
    }

} // namespace mg
