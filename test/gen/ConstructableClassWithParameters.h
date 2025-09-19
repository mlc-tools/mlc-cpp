#ifndef __mg_ConstructableClassWithParameters_h__
#define __mg_ConstructableClassWithParameters_h__

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


    class ConstructableClassWithParameters
    {
    public:
        ConstructableClassWithParameters(int a=0, int b=0);
        ~ConstructableClassWithParameters();
void retain();
int release();
bool operator ==(const ConstructableClassWithParameters& rhs) const;
bool operator !=(const ConstructableClassWithParameters& rhs) const;
 ConstructableClassWithParameters(const ConstructableClassWithParameters& rhs);
const ConstructableClassWithParameters& operator =(const ConstructableClassWithParameters& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

int a;
int b;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ConstructableClassWithParameters_h__
