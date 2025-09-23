#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataStatUpgradeVisual.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataStatUpgradeVisual::TYPE("DataStatUpgradeVisual");

    DataStatUpgradeVisual::DataStatUpgradeVisual()
    : title("")
    , desc("")
    , icon("")
    , _reference_counter(1)
    {

    }

    DataStatUpgradeVisual::~DataStatUpgradeVisual()
    {
    }

    void DataStatUpgradeVisual::retain()
    {
        ++this->_reference_counter;
    }

    int DataStatUpgradeVisual::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool DataStatUpgradeVisual::operator ==(const DataStatUpgradeVisual& rhs) const
    {
        bool result = true;
        result = result && this->title == rhs.title;
        result = result && this->desc == rhs.desc;
        result = result && this->icon == rhs.icon;
        return result;
    }

    bool DataStatUpgradeVisual::operator !=(const DataStatUpgradeVisual& rhs) const
    {
        return !(*this == rhs);
    }

    DataStatUpgradeVisual::DataStatUpgradeVisual(const DataStatUpgradeVisual& rhs)
    {
        this->operator=(rhs);
    }

    const DataStatUpgradeVisual& DataStatUpgradeVisual::operator =(const DataStatUpgradeVisual& rhs)
    {
        this->title = rhs.title;
        this->desc = rhs.desc;
        this->icon = rhs.icon;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataStatUpgradeVisual::get_type() const
    {
        return DataStatUpgradeVisual::TYPE;
    }

    void DataStatUpgradeVisual::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(title, "title", std::string(""));
        serializer.serialize(desc, "desc", std::string(""));
        serializer.serialize(icon, "icon", std::string(""));
    }

    void DataStatUpgradeVisual::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(title, "title", std::string(""));
        deserializer.deserialize(desc, "desc", std::string(""));
        deserializer.deserialize(icon, "icon", std::string(""));
    }

    void DataStatUpgradeVisual::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(title, "title", std::string(""));
        serializer.serialize(desc, "desc", std::string(""));
        serializer.serialize(icon, "icon", std::string(""));
    }

    void DataStatUpgradeVisual::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(title, "title", std::string(""));
        deserializer.deserialize(desc, "desc", std::string(""));
        deserializer.deserialize(icon, "icon", std::string(""));
    }

} //namespace mg
