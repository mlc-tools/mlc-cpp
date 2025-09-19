#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "LadderLevels.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string LadderLevels::TYPE("LadderLevels");

    
    LadderLevels::LadderLevels()
    : name()
, _reference_counter(1)
{
    
    }

    LadderLevels::~LadderLevels(){}


void LadderLevels::retain()
{
++this->_reference_counter;
}

int LadderLevels::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool LadderLevels::operator ==(const LadderLevels& rhs) const
{
bool result = true;
result = result && this->name == rhs.name;
return result;
}

bool LadderLevels::operator !=(const LadderLevels& rhs) const
{

return !(*this == rhs);
}

 LadderLevels::LadderLevels(const LadderLevels& rhs)
{

this->operator=(rhs);
}

const LadderLevels& LadderLevels::operator =(const LadderLevels& rhs)
{

this->name = rhs.name;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string LadderLevels::get_type() const
{
return LadderLevels::TYPE;
}

void LadderLevels::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(name, "name", std::string());
}

void LadderLevels::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(name, "name", std::string());
}

void LadderLevels::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(name, "name", std::string());
}

void LadderLevels::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(name, "name", std::string());
}

} // namespace mg
