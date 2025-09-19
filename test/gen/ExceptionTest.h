#ifndef __mg_ExceptionTest_h__
#define __mg_ExceptionTest_h__

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


    class ExceptionTest
    {
    public:
        ExceptionTest();
        ~ExceptionTest();
static bool test();
void retain();
int release();
bool operator ==(const ExceptionTest& rhs) const;
bool operator !=(const ExceptionTest& rhs) const;
 ExceptionTest(const ExceptionTest& rhs);
const ExceptionTest& operator =(const ExceptionTest& rhs);
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

#endif // __mg_ExceptionTest_h__
