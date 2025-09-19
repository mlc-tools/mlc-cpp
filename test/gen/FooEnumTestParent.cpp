#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "FooEnumTestParent.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string FooEnumTestParent::TYPE("FooEnumTestParent");

    
    FooEnumTestParent::FooEnumTestParent()
    : _reference_counter(1)
{
    
    }

    FooEnumTestParent::~FooEnumTestParent(){}


void FooEnumTestParent::dummy()
{

    
}

void FooEnumTestParent::retain()
{
++this->_reference_counter;
}

int FooEnumTestParent::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool FooEnumTestParent::operator ==(const FooEnumTestParent& rhs) const
{
bool result = true;
return result;
}

bool FooEnumTestParent::operator !=(const FooEnumTestParent& rhs) const
{

return !(*this == rhs);
}

 FooEnumTestParent::FooEnumTestParent(const FooEnumTestParent& rhs)
{

this->operator=(rhs);
}

const FooEnumTestParent& FooEnumTestParent::operator =(const FooEnumTestParent& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string FooEnumTestParent::get_type() const
{
return FooEnumTestParent::TYPE;
}

void FooEnumTestParent::serialize_xml(SerializerXml& serializer) const
{

}

void FooEnumTestParent::deserialize_xml(DeserializerXml& deserializer)
{

}

void FooEnumTestParent::serialize_json(SerializerJson& serializer) const
{

}

void FooEnumTestParent::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
