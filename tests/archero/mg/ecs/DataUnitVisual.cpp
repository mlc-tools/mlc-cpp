#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataUnitVisual.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataUnitVisual::TYPE("DataUnitVisual");

    DataUnitVisual::DataUnitVisual()
    : path_to_view("")
    , _reference_counter(1)
    {

    }

    DataUnitVisual::~DataUnitVisual()
    {
    }

    void DataUnitVisual::retain()
    {
        ++this->_reference_counter;
    }

    int DataUnitVisual::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataUnitVisual::operator ==(const DataUnitVisual& rhs) const
    {
        bool result = true;
        result = result && this->path_to_view == rhs.path_to_view;
        return result;
    }

    bool DataUnitVisual::operator !=(const DataUnitVisual& rhs) const
    {
        return !(*this == rhs);
    }

    DataUnitVisual::DataUnitVisual(const DataUnitVisual& rhs)
    {
        this->operator=(rhs);
    }

    const DataUnitVisual& DataUnitVisual::operator =(const DataUnitVisual& rhs)
    {
        this->path_to_view = rhs.path_to_view;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataUnitVisual::get_type() const
    {
        return DataUnitVisual::TYPE;
    }

    void DataUnitVisual::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(path_to_view, "path_to_view", std::string(""));
    }

    void DataUnitVisual::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(path_to_view, "path_to_view", std::string(""));
    }

    void DataUnitVisual::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(path_to_view, "path_to_view", std::string(""));
    }

    void DataUnitVisual::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(path_to_view, "path_to_view", std::string(""));
    }

} //namespace mg
