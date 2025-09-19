#ifndef __mg_FunctionTemplatesSomeOtherClass_h__
#define __mg_FunctionTemplatesSomeOtherClass_h__

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


    class FunctionTemplatesSomeOtherClass
    {
    public:
        FunctionTemplatesSomeOtherClass();
        ~FunctionTemplatesSomeOtherClass();
void some_method();
void retain();
int release();
bool operator ==(const FunctionTemplatesSomeOtherClass& rhs) const;
bool operator !=(const FunctionTemplatesSomeOtherClass& rhs) const;
 FunctionTemplatesSomeOtherClass(const FunctionTemplatesSomeOtherClass& rhs);
const FunctionTemplatesSomeOtherClass& operator =(const FunctionTemplatesSomeOtherClass& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_FunctionTemplatesSomeOtherClass_h__
