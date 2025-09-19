#ifndef __mg_TestIncludes_h__
#define __mg_TestIncludes_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestIncludes
    {
    public:
        TestIncludes();
        ~TestIncludes();
void get();
std::vector<int> get_vector();
void retain();
int release();
bool operator ==(const TestIncludes& rhs) const;
bool operator !=(const TestIncludes& rhs) const;
 TestIncludes(const TestIncludes& rhs);
const TestIncludes& operator =(const TestIncludes& rhs);
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

#endif // __mg_TestIncludes_h__
