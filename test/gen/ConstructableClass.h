#ifndef __mg_ConstructableClass_h__
#define __mg_ConstructableClass_h__

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


    class ConstructableClass
    {
    public:
        ConstructableClass();
        ~ConstructableClass();
void retain();
int release();
bool operator ==(const ConstructableClass& rhs) const;
bool operator !=(const ConstructableClass& rhs) const;
 ConstructableClass(const ConstructableClass& rhs);
const ConstructableClass& operator =(const ConstructableClass& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

int a;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ConstructableClass_h__
