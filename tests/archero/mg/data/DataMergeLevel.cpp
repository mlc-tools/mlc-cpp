#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataMergeLevel.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataMergeLevel::TYPE("DataMergeLevel");

    DataMergeLevel::DataMergeLevel()
    : name("")
    , visual()
    , index(0)
    , next(nullptr)
    , require_items_to_merge()
    , count_items_to_merge(0)
    , _reference_counter(1)
    {

    }

    DataMergeLevel::~DataMergeLevel()
    {
    }

    void DataMergeLevel::retain()
    {
        ++this->_reference_counter;
    }

    int DataMergeLevel::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataMergeLevel::operator ==(const DataMergeLevel& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->visual == rhs.visual;
        result = result && this->index == rhs.index;
        result = result && this->require_items_to_merge == rhs.require_items_to_merge;
        result = result && this->count_items_to_merge == rhs.count_items_to_merge;
        return result;
    }

    bool DataMergeLevel::operator !=(const DataMergeLevel& rhs) const
    {
        return !(*this == rhs);
    }

    DataMergeLevel::DataMergeLevel(const DataMergeLevel& rhs)
    {
        this->operator=(rhs);
    }

    const DataMergeLevel& DataMergeLevel::operator =(const DataMergeLevel& rhs)
    {
        this->name = rhs.name;
        this->visual = rhs.visual;
        this->index = rhs.index;
        this->next = rhs.next;
        this->require_items_to_merge = rhs.require_items_to_merge;
        this->count_items_to_merge = rhs.count_items_to_merge;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataMergeLevel::get_type() const
    {
        return DataMergeLevel::TYPE;
    }

    void DataMergeLevel::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(visual, "visual");
        serializer.serialize(index, "index", int(0));
        serializer.serialize(next, "next");
        serializer.serialize(require_items_to_merge, "require_items_to_merge");
        serializer.serialize(count_items_to_merge, "count_items_to_merge", int(0));
    }

    void DataMergeLevel::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(visual, "visual");
        deserializer.deserialize(index, "index", int(0));
        deserializer.deserialize(next, "next");
        deserializer.deserialize(require_items_to_merge, "require_items_to_merge");
        deserializer.deserialize(count_items_to_merge, "count_items_to_merge", int(0));
    }

    void DataMergeLevel::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(visual, "visual");
        serializer.serialize(index, "index", int(0));
        serializer.serialize(next, "next");
        serializer.serialize(require_items_to_merge, "require_items_to_merge");
        serializer.serialize(count_items_to_merge, "count_items_to_merge", int(0));
    }

    void DataMergeLevel::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(visual, "visual");
        deserializer.deserialize(index, "index", int(0));
        deserializer.deserialize(next, "next");
        deserializer.deserialize(require_items_to_merge, "require_items_to_merge");
        deserializer.deserialize(count_items_to_merge, "count_items_to_merge", int(0));
    }

} //namespace mg
