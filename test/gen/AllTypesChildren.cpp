#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "AllTypesChildren.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string AllTypesChildren::TYPE("AllTypesChildren");

    
    AllTypesChildren::AllTypesChildren()
    : value(0)
, _reference_counter(1)
{
    
    }

    AllTypesChildren::~AllTypesChildren(){}


void AllTypesChildren::retain()
{
++this->_reference_counter;
}

int AllTypesChildren::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool AllTypesChildren::operator ==(const AllTypesChildren& rhs) const
{
bool result = true;
result = result && this->value == rhs.value;
return result;
}

bool AllTypesChildren::operator !=(const AllTypesChildren& rhs) const
{

return !(*this == rhs);
}

 AllTypesChildren::AllTypesChildren(const AllTypesChildren& rhs)
{

this->operator=(rhs);
}

const AllTypesChildren& AllTypesChildren::operator =(const AllTypesChildren& rhs)
{

this->value = rhs.value;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string AllTypesChildren::get_type() const
{
return AllTypesChildren::TYPE;
}

void AllTypesChildren::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(value, "value", int(0));
}

void AllTypesChildren::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(value, "value", int(0));
}

void AllTypesChildren::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(value, "value", int(0));
}

void AllTypesChildren::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(value, "value", int(0));
}

} // namespace mg
