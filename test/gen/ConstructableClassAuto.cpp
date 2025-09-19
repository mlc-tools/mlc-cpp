#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "ConstructableClassAuto.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string ConstructableClassAuto::TYPE("ConstructableClassAuto");

    
    ConstructableClassAuto::ConstructableClassAuto()
    : a(-1)
, b(-1)
, _reference_counter(1)
{
    
    }

    ConstructableClassAuto::~ConstructableClassAuto(){}


void ConstructableClassAuto::retain()
{
++this->_reference_counter;
}

int ConstructableClassAuto::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool ConstructableClassAuto::operator ==(const ConstructableClassAuto& rhs) const
{
bool result = true;
result = result && this->a == rhs.a;
result = result && this->b == rhs.b;
return result;
}

bool ConstructableClassAuto::operator !=(const ConstructableClassAuto& rhs) const
{

return !(*this == rhs);
}

 ConstructableClassAuto::ConstructableClassAuto(const ConstructableClassAuto& rhs)
{

this->operator=(rhs);
}

const ConstructableClassAuto& ConstructableClassAuto::operator =(const ConstructableClassAuto& rhs)
{

this->a = rhs.a;
this->b = rhs.b;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string ConstructableClassAuto::get_type() const
{
return ConstructableClassAuto::TYPE;
}

void ConstructableClassAuto::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(a, "a", int(-1));
serializer.serialize(b, "b", int(-1));
}

void ConstructableClassAuto::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(a, "a", int(-1));
deserializer.deserialize(b, "b", int(-1));
}

void ConstructableClassAuto::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(a, "a", int(-1));
serializer.serialize(b, "b", int(-1));
}

void ConstructableClassAuto::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(a, "a", int(-1));
deserializer.deserialize(b, "b", int(-1));
}

} // namespace mg
