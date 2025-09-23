#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataLadderLevels.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataLadderLevels::TYPE("DataLadderLevels");

    DataLadderLevels::DataLadderLevels()
    : name("")
    , values()
    , _reference_counter(1)
    {

    }

    DataLadderLevels::~DataLadderLevels()
    {
    }

    float DataLadderLevels::get_value(int level) const
    {
        assert(level < list_size(this->values));
        assert(list_size(this->values) > 0);
        auto index = std::min(level, list_size(this->values)-1);
        return this->values.at(index);
    }

    int DataLadderLevels::get_level(int exp) const
    {
        int min_index = 0;
        int max_index = list_size(this->values);
        int index = round(std::ceil((max_index + min_index) / 2));

        while(max_index - min_index > 1)
        {
            auto value = this->values.at(index);
            if(exp == value)
            {
                return index;
            }
            else if(exp > value)
            {
                min_index = index;
            }
            else if(exp < value)
            {
                max_index = index;
            }
            index = round(std::floor((max_index + min_index) / 2.0f));
        }
        return index;
    }

    float DataLadderLevels::get_progress_to_next_level(int exp) const
    {
        auto level = this->get_level(exp);
        if(level+1 >= list_size(this->values))
        {
            return 1.f;
        }
        auto next = level + 1;
        auto diff = this->values[next] - this->values[level];
        auto collected = exp - this->values[level];
        auto progress = collected * 1.0f / diff;
        return progress;
    }

    int DataLadderLevels::get_progress_to_next_level_int(int exp) const
    {
        auto level = this->get_level(exp);
        if(level+1 >= list_size(this->values))
        {
            return 1.f;
        }
        auto collected = exp - this->values[level];
        return collected;
    }

    int DataLadderLevels::get_count_to_next_level_int(int exp) const
    {
        auto level = this->get_level(exp);
        if(level+1 >= list_size(this->values))
        {
            return 1.f;
        }
        auto next = level + 1;
        auto diff = this->values[next] - this->values[level];
        return diff;
    }

    void DataLadderLevels::retain()
    {
        ++this->_reference_counter;
    }

    int DataLadderLevels::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool DataLadderLevels::operator ==(const DataLadderLevels& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->values == rhs.values;
        return result;
    }

    bool DataLadderLevels::operator !=(const DataLadderLevels& rhs) const
    {
        return !(*this == rhs);
    }

    DataLadderLevels::DataLadderLevels(const DataLadderLevels& rhs)
    {
        this->operator=(rhs);
    }

    const DataLadderLevels& DataLadderLevels::operator =(const DataLadderLevels& rhs)
    {
        this->name = rhs.name;
        this->values = rhs.values;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataLadderLevels::get_type() const
    {
        return DataLadderLevels::TYPE;
    }

    void DataLadderLevels::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(values, "values");
    }

    void DataLadderLevels::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(values, "values");
    }

    void DataLadderLevels::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(values, "values");
    }

    void DataLadderLevels::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(values, "values");
    }

} //namespace mg
