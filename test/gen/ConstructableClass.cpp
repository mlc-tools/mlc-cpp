#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "ConstructableClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string ConstructableClass::TYPE("ConstructableClass");

    
    ConstructableClass::ConstructableClass()
    : a(-1)
, _reference_counter(1)
{
    
        this->a = 1;
    
    }

    ConstructableClass::~ConstructableClass(){}


void ConstructableClass::retain()
{
++this->_reference_counter;
}

int ConstructableClass::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool ConstructableClass::operator ==(const ConstructableClass& rhs) const
{
bool result = true;
result = result && this->a == rhs.a;
return result;
}

bool ConstructableClass::operator !=(const ConstructableClass& rhs) const
{

return !(*this == rhs);
}

 ConstructableClass::ConstructableClass(const ConstructableClass& rhs)
{

this->operator=(rhs);
}

const ConstructableClass& ConstructableClass::operator =(const ConstructableClass& rhs)
{

this->a = rhs.a;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string ConstructableClass::get_type() const
{
return ConstructableClass::TYPE;
}

void ConstructableClass::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(a, "a", int(-1));
}

void ConstructableClass::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(a, "a", int(-1));
}

void ConstructableClass::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(a, "a", int(-1));
}

void ConstructableClass::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(a, "a", int(-1));
}

} // namespace mg
