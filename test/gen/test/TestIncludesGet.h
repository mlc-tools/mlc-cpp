#ifndef __mg_TestIncludesGet_h__
#define __mg_TestIncludesGet_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../core/CommandBase.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestIncludesGet
    {
    public:
        TestIncludesGet();
        ~TestIncludesGet();
intrusive_ptr<CommandBase> get();
void retain();
int release();
bool operator ==(const TestIncludesGet& rhs) const;
bool operator !=(const TestIncludesGet& rhs) const;
 TestIncludesGet(const TestIncludesGet& rhs);
const TestIncludesGet& operator =(const TestIncludesGet& rhs);
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

#endif // __mg_TestIncludesGet_h__
