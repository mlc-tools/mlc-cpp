#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CloneObject.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CloneObject::TYPE("CloneObject");

    
    CloneObject::CloneObject()
    : t(0)
, _reference_counter(1)
{
    
    }

    CloneObject::~CloneObject(){}


void CloneObject::foo()
{

    
}

void CloneObject::retain()
{
++this->_reference_counter;
}

int CloneObject::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool CloneObject::operator ==(const CloneObject& rhs) const
{
bool result = true;
result = result && this->t == rhs.t;
return result;
}

bool CloneObject::operator !=(const CloneObject& rhs) const
{

return !(*this == rhs);
}

 CloneObject::CloneObject(const CloneObject& rhs)
{

this->operator=(rhs);
}

const CloneObject& CloneObject::operator =(const CloneObject& rhs)
{

this->t = rhs.t;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string CloneObject::get_type() const
{
return CloneObject::TYPE;
}

void CloneObject::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(t, "t", int(0));
}

void CloneObject::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(t, "t", int(0));
}

void CloneObject::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(t, "t", int(0));
}

void CloneObject::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(t, "t", int(0));
}

} // namespace mg
