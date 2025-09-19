#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataNonExist.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataNonExist::TYPE("DataNonExist");

    
    DataNonExist::DataNonExist()
    : name()
, _reference_counter(1)
{
    
    }

    DataNonExist::~DataNonExist(){}


void DataNonExist::retain()
{
++this->_reference_counter;
}

int DataNonExist::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool DataNonExist::operator ==(const DataNonExist& rhs) const
{
bool result = true;
result = result && this->name == rhs.name;
return result;
}

bool DataNonExist::operator !=(const DataNonExist& rhs) const
{

return !(*this == rhs);
}

 DataNonExist::DataNonExist(const DataNonExist& rhs)
{

this->operator=(rhs);
}

const DataNonExist& DataNonExist::operator =(const DataNonExist& rhs)
{

this->name = rhs.name;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string DataNonExist::get_type() const
{
return DataNonExist::TYPE;
}

void DataNonExist::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(name, "name", std::string());
}

void DataNonExist::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(name, "name", std::string());
}

void DataNonExist::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(name, "name", std::string());
}

void DataNonExist::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(name, "name", std::string());
}

} // namespace mg
