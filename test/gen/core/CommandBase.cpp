#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CommandBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CommandBase::TYPE("CommandBase");

    
    CommandBase::CommandBase()
    : user_id(0)
, current_time(0)
, _reference_counter(1)
{
    
    }

    CommandBase::~CommandBase(){}


void CommandBase::retain()
{
++this->_reference_counter;
}

int CommandBase::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool CommandBase::operator ==(const CommandBase& rhs) const
{
bool result = true;
result = result && this->user_id == rhs.user_id;
result = result && this->current_time == rhs.current_time;
return result;
}

bool CommandBase::operator !=(const CommandBase& rhs) const
{

return !(*this == rhs);
}

 CommandBase::CommandBase(const CommandBase& rhs)
{

this->operator=(rhs);
}

const CommandBase& CommandBase::operator =(const CommandBase& rhs)
{

this->user_id = rhs.user_id;
this->current_time = rhs.current_time;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string CommandBase::get_type() const
{
return CommandBase::TYPE;
}

void CommandBase::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(user_id, "user_id", int(0));
serializer.serialize(current_time, "current_time", int(0));
}

void CommandBase::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(user_id, "user_id", int(0));
deserializer.deserialize(current_time, "current_time", int(0));
}

void CommandBase::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(user_id, "user_id", int(0));
serializer.serialize(current_time, "current_time", int(0));
}

void CommandBase::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(user_id, "user_id", int(0));
deserializer.deserialize(current_time, "current_time", int(0));
}

} // namespace mg
