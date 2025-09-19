#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "ConstructableClassWithParameters.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string ConstructableClassWithParameters::TYPE("ConstructableClassWithParameters");

    
    ConstructableClassWithParameters::ConstructableClassWithParameters(int a, int b)
    : a(-1)
, b(-1)
, _reference_counter(1)
{
    
        this->a = a;
        this->b = b;
    
    }

    ConstructableClassWithParameters::~ConstructableClassWithParameters(){}


void ConstructableClassWithParameters::retain()
{
++this->_reference_counter;
}

int ConstructableClassWithParameters::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool ConstructableClassWithParameters::operator ==(const ConstructableClassWithParameters& rhs) const
{
bool result = true;
result = result && this->a == rhs.a;
result = result && this->b == rhs.b;
return result;
}

bool ConstructableClassWithParameters::operator !=(const ConstructableClassWithParameters& rhs) const
{

return !(*this == rhs);
}

 ConstructableClassWithParameters::ConstructableClassWithParameters(const ConstructableClassWithParameters& rhs)
{

this->operator=(rhs);
}

const ConstructableClassWithParameters& ConstructableClassWithParameters::operator =(const ConstructableClassWithParameters& rhs)
{

this->a = rhs.a;
this->b = rhs.b;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string ConstructableClassWithParameters::get_type() const
{
return ConstructableClassWithParameters::TYPE;
}

void ConstructableClassWithParameters::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(a, "a", int(-1));
serializer.serialize(b, "b", int(-1));
}

void ConstructableClassWithParameters::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(a, "a", int(-1));
deserializer.deserialize(b, "b", int(-1));
}

void ConstructableClassWithParameters::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(a, "a", int(-1));
serializer.serialize(b, "b", int(-1));
}

void ConstructableClassWithParameters::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(a, "a", int(-1));
deserializer.deserialize(b, "b", int(-1));
}

} // namespace mg
