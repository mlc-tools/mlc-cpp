#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "VisualItem.h"
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string VisualItem::TYPE("VisualItem");

    VisualItem::VisualItem()
    : name("")
    , desc("")
    , icon("")
    , _reference_counter(1)
    {

    }

    VisualItem::~VisualItem()
    {
    }

    void VisualItem::retain()
    {
        ++this->_reference_counter;
    }

    int VisualItem::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool VisualItem::operator ==(const VisualItem& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->desc == rhs.desc;
        result = result && this->icon == rhs.icon;
        return result;
    }

    bool VisualItem::operator !=(const VisualItem& rhs) const
    {
        return !(*this == rhs);
    }

    VisualItem::VisualItem(const VisualItem& rhs)
    {
        this->operator=(rhs);
    }

    const VisualItem& VisualItem::operator =(const VisualItem& rhs)
    {
        this->name = rhs.name;
        this->desc = rhs.desc;
        this->icon = rhs.icon;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string VisualItem::get_type() const
    {
        return VisualItem::TYPE;
    }

    void VisualItem::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(desc, "desc", std::string(""));
        serializer.serialize(icon, "icon", std::string(""));
    }

    void VisualItem::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(desc, "desc", std::string(""));
        deserializer.deserialize(icon, "icon", std::string(""));
    }

    void VisualItem::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(desc, "desc", std::string(""));
        serializer.serialize(icon, "icon", std::string(""));
    }

    void VisualItem::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(desc, "desc", std::string(""));
        deserializer.deserialize(icon, "icon", std::string(""));
    }

} //namespace mg
