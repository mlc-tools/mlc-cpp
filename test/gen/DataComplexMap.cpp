#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataComplexMap.h"
#include <map>
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string DataComplexMap::TYPE("DataComplexMap");

    
    DataComplexMap::DataComplexMap()
    : name()
, complex_map()
, _reference_counter(1)
{
    
    }

    DataComplexMap::~DataComplexMap(){}


void DataComplexMap::foo()
{

    
}

void DataComplexMap::bar_private()
{

    
}

void DataComplexMap::retain()
{
++this->_reference_counter;
}

int DataComplexMap::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool DataComplexMap::operator ==(const DataComplexMap& rhs) const
{
bool result = true;
result = result && this->name == rhs.name;
result = result && this->complex_map == rhs.complex_map;
return result;
}

bool DataComplexMap::operator !=(const DataComplexMap& rhs) const
{

return !(*this == rhs);
}

 DataComplexMap::DataComplexMap(const DataComplexMap& rhs)
{

this->operator=(rhs);
}

const DataComplexMap& DataComplexMap::operator =(const DataComplexMap& rhs)
{

this->name = rhs.name;
this->complex_map = rhs.complex_map;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string DataComplexMap::get_type() const
{
return DataComplexMap::TYPE;
}

void DataComplexMap::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(name, "name", std::string());
serializer.serialize(complex_map, "complex_map");
}

void DataComplexMap::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(name, "name", std::string());
deserializer.deserialize(complex_map, "complex_map");
}

void DataComplexMap::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(name, "name", std::string());
serializer.serialize(complex_map, "complex_map");
}

void DataComplexMap::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(name, "name", std::string());
deserializer.deserialize(complex_map, "complex_map");
}

} // namespace mg
