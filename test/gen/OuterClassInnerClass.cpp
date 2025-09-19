#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "OuterClassInnerClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string OuterClassInnerClass::TYPE("OuterClassInnerClass");

    
    OuterClassInnerClass::OuterClassInnerClass()
    : _reference_counter(1)
{
    
    }

    OuterClassInnerClass::~OuterClassInnerClass(){}


bool OuterClassInnerClass::func()
{

            return true;
        
}

void OuterClassInnerClass::retain()
{
++this->_reference_counter;
}

int OuterClassInnerClass::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool OuterClassInnerClass::operator ==(const OuterClassInnerClass& rhs) const
{
bool result = true;
return result;
}

bool OuterClassInnerClass::operator !=(const OuterClassInnerClass& rhs) const
{

return !(*this == rhs);
}

 OuterClassInnerClass::OuterClassInnerClass(const OuterClassInnerClass& rhs)
{

this->operator=(rhs);
}

const OuterClassInnerClass& OuterClassInnerClass::operator =(const OuterClassInnerClass& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string OuterClassInnerClass::get_type() const
{
return OuterClassInnerClass::TYPE;
}

void OuterClassInnerClass::serialize_xml(SerializerXml& serializer) const
{

}

void OuterClassInnerClass::deserialize_xml(DeserializerXml& deserializer)
{

}

void OuterClassInnerClass::serialize_json(SerializerJson& serializer) const
{

}

void OuterClassInnerClass::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
