#ifndef __mg_TestEnumValue1_h__
#define __mg_TestEnumValue1_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "TestEnum.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestEnumValue1 : public TestEnum
    {
    public:
        TestEnumValue1();
        ~TestEnumValue1();
bool operator ==(const TestEnumValue1& rhs) const;
bool operator !=(const TestEnumValue1& rhs) const;
 TestEnumValue1(const TestEnumValue1& rhs);
const TestEnumValue1& operator =(const TestEnumValue1& rhs);
virtual std::string get_type() const;
virtual void serialize_xml(SerializerXml& serializer) const;
virtual void deserialize_xml(DeserializerXml& deserializer);
virtual void serialize_json(SerializerJson& serializer) const;
virtual void deserialize_json(DeserializerJson& deserializer);

int parameter;
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestEnumValue1_h__
