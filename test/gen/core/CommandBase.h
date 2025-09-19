#ifndef __mg_CommandBase_h__
#define __mg_CommandBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class CommandBase
    {
    public:
        CommandBase();
        ~CommandBase();
void retain();
int release();
bool operator ==(const CommandBase& rhs) const;
bool operator !=(const CommandBase& rhs) const;
 CommandBase(const CommandBase& rhs);
const CommandBase& operator =(const CommandBase& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

int user_id;
int current_time;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_CommandBase_h__
