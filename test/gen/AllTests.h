#ifndef __mg_AllTests_h__
#define __mg_AllTests_h__

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
class Logger;

    class AllTests
    {
    public:
        AllTests();
        ~AllTests();
static bool run(Logger* logger);
void retain();
int release();
bool operator ==(const AllTests& rhs) const;
bool operator !=(const AllTests& rhs) const;
 AllTests(const AllTests& rhs);
const AllTests& operator =(const AllTests& rhs);
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

#endif // __mg_AllTests_h__
