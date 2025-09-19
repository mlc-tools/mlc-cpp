#ifndef __mg_TestTranslatesModel_h__
#define __mg_TestTranslatesModel_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "data/DataUnit.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestTranslatesModel
    {
    public:
        TestTranslatesModel();
        ~TestTranslatesModel();
void retain();
int release();
bool operator ==(const TestTranslatesModel& rhs) const;
bool operator !=(const TestTranslatesModel& rhs) const;
 TestTranslatesModel(const TestTranslatesModel& rhs);
const TestTranslatesModel& operator =(const TestTranslatesModel& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

const DataUnit* data;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestTranslatesModel_h__
