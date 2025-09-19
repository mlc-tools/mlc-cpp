#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "Translates.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string Translates::TYPE("Translates");

    
    Translates::Translates()
    : _reference_counter(1)
{
    
    }

    Translates::~Translates(){}


void Translates::dummy_function()
{


    
}

void Translates::new_function()
{


    
}

void Translates::retain()
{
++this->_reference_counter;
}

int Translates::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool Translates::operator ==(const Translates& rhs) const
{
bool result = true;
return result;
}

bool Translates::operator !=(const Translates& rhs) const
{

return !(*this == rhs);
}

 Translates::Translates(const Translates& rhs)
{

this->operator=(rhs);
}

const Translates& Translates::operator =(const Translates& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string Translates::get_type() const
{
return Translates::TYPE;
}

void Translates::serialize_xml(SerializerXml& serializer) const
{

}

void Translates::deserialize_xml(DeserializerXml& deserializer)
{

}

void Translates::serialize_json(SerializerJson& serializer) const
{

}

void Translates::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
