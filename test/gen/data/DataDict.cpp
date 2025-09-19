#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../Resource.h"
#include "DataDict.h"
#include <map>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataDict::TYPE("DataDict");

    
    DataDict::DataDict()
    : name()
, string_with_def_value("default")
, resources()
, _reference_counter(1)
{
    
    }

    DataDict::~DataDict(){}


void DataDict::retain()
{
++this->_reference_counter;
}

int DataDict::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool DataDict::operator ==(const DataDict& rhs) const
{
bool result = true;
result = result && this->name == rhs.name;
result = result && this->string_with_def_value == rhs.string_with_def_value;
result = result && this->resources == rhs.resources;
return result;
}

bool DataDict::operator !=(const DataDict& rhs) const
{

return !(*this == rhs);
}

 DataDict::DataDict(const DataDict& rhs)
{

this->operator=(rhs);
}

const DataDict& DataDict::operator =(const DataDict& rhs)
{

this->name = rhs.name;
this->string_with_def_value = rhs.string_with_def_value;
this->resources = rhs.resources;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string DataDict::get_type() const
{
return DataDict::TYPE;
}

void DataDict::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(name, "name", std::string());
serializer.serialize(string_with_def_value, "string_with_def_value", std::string("default"));
serializer.serialize(resources, "resources");
}

void DataDict::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(name, "name", std::string());
deserializer.deserialize(string_with_def_value, "string_with_def_value", std::string("default"));
deserializer.deserialize(resources, "resources");
}

void DataDict::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(name, "name", std::string());
serializer.serialize(string_with_def_value, "string_with_def_value", std::string("default"));
serializer.serialize(resources, "resources");
}

void DataDict::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(name, "name", std::string());
deserializer.deserialize(string_with_def_value, "string_with_def_value", std::string("default"));
deserializer.deserialize(resources, "resources");
}

} // namespace mg
