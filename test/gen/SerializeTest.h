#ifndef __mg_SerializeTest_h__
#define __mg_SerializeTest_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "data/DataUnit.h"
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class SerializeTest
    {
    public:
        SerializeTest();
        ~SerializeTest();
void retain();
int release();
bool operator ==(const SerializeTest& rhs) const;
bool operator !=(const SerializeTest& rhs) const;
 SerializeTest(const SerializeTest& rhs);
const SerializeTest& operator =(const SerializeTest& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

std::vector<intrusive_ptr<DataUnit>> list_of_data;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_SerializeTest_h__
