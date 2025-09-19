#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SkillBase.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string SkillBase::TYPE("SkillBase");

    
    SkillBase::SkillBase()
    : _reference_counter(1)
{
    
    }

    SkillBase::~SkillBase(){}


void SkillBase::retain()
{
++this->_reference_counter;
}

int SkillBase::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool SkillBase::operator ==(const SkillBase& rhs) const
{
bool result = true;
return result;
}

bool SkillBase::operator !=(const SkillBase& rhs) const
{

return !(*this == rhs);
}

 SkillBase::SkillBase(const SkillBase& rhs)
{

this->operator=(rhs);
}

const SkillBase& SkillBase::operator =(const SkillBase& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string SkillBase::get_type() const
{
return SkillBase::TYPE;
}

void SkillBase::serialize_xml(SerializerXml& serializer) const
{

}

void SkillBase::deserialize_xml(DeserializerXml& deserializer)
{

}

void SkillBase::serialize_json(SerializerJson& serializer) const
{

}

void SkillBase::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
