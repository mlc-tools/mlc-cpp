#ifndef __mg_CloneObject_h__
#define __mg_CloneObject_h__

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


    class CloneObject
    {
    public:
        CloneObject();
        ~CloneObject();
void foo();
void retain();
int release();
bool operator ==(const CloneObject& rhs) const;
bool operator !=(const CloneObject& rhs) const;
 CloneObject(const CloneObject& rhs);
const CloneObject& operator =(const CloneObject& rhs);
virtual std::string get_type() const;
virtual void serialize_xml(SerializerXml& serializer) const;
virtual void deserialize_xml(DeserializerXml& deserializer);
virtual void serialize_json(SerializerJson& serializer) const;
virtual void deserialize_json(DeserializerJson& deserializer);

int t;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_CloneObject_h__
