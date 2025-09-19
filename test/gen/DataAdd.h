#ifndef __mg_DataAdd_h__
#define __mg_DataAdd_h__

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


    class DataAdd
    {
    public:
        DataAdd();
        ~DataAdd();
void test();
void retain();
int release();
bool operator ==(const DataAdd& rhs) const;
bool operator !=(const DataAdd& rhs) const;
 DataAdd(const DataAdd& rhs);
const DataAdd& operator =(const DataAdd& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

std::string name;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_DataAdd_h__
