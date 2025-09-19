#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "Intrusive.h"
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string Intrusive::TYPE("Intrusive");

    
    Intrusive::Intrusive()
    : _reference_counter(1)
{
    
    }

    Intrusive::~Intrusive(){}


void Intrusive::add_self_to_list(std::vector<intrusive_ptr<Intrusive>>& list)
{

        auto r = this;
        list_push(list, r);
    
}

void Intrusive::remove_self_from_list(std::vector<intrusive_ptr<Intrusive>>& list)
{

        auto r = this;
        list_remove(list, r);
    
}

void Intrusive::retain()
{
++this->_reference_counter;
}

int Intrusive::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool Intrusive::operator ==(const Intrusive& rhs) const
{
bool result = true;
return result;
}

bool Intrusive::operator !=(const Intrusive& rhs) const
{

return !(*this == rhs);
}

 Intrusive::Intrusive(const Intrusive& rhs)
{

this->operator=(rhs);
}

const Intrusive& Intrusive::operator =(const Intrusive& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string Intrusive::get_type() const
{
return Intrusive::TYPE;
}

void Intrusive::serialize_xml(SerializerXml& serializer) const
{

}

void Intrusive::deserialize_xml(DeserializerXml& deserializer)
{

}

void Intrusive::serialize_json(SerializerJson& serializer) const
{

}

void Intrusive::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
