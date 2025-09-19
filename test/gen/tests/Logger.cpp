#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Logger.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Logger::TYPE("Logger");

    
    Logger::Logger()
    : _reference_counter(1)
{
    
    }

    Logger::~Logger(){}


void Logger::log(const std::string& message)
{

        this->message(message);
    
}

void Logger::retain()
{
++this->_reference_counter;
}

int Logger::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool Logger::operator ==(const Logger& rhs) const
{
bool result = true;
return result;
}

bool Logger::operator !=(const Logger& rhs) const
{

return !(*this == rhs);
}

 Logger::Logger(const Logger& rhs)
{

this->operator=(rhs);
}

const Logger& Logger::operator =(const Logger& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string Logger::get_type() const
{
return Logger::TYPE;
}

void Logger::serialize_xml(SerializerXml& serializer) const
{

}

void Logger::deserialize_xml(DeserializerXml& deserializer)
{

}

void Logger::serialize_json(SerializerJson& serializer) const
{

}

void Logger::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
