#ifndef __mg_TestEnumValue2_h__
#define __mg_TestEnumValue2_h__

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


    class TestEnumValue2 : public TestEnum
    {
    public:
        TestEnumValue2();
        ~TestEnumValue2();
bool operator ==(const TestEnumValue2& rhs) const;
bool operator !=(const TestEnumValue2& rhs) const;
 TestEnumValue2(const TestEnumValue2& rhs);
const TestEnumValue2& operator =(const TestEnumValue2& rhs);
virtual std::string get_type() const;
virtual void serialize_xml(SerializerXml& serializer) const;
virtual void deserialize_xml(DeserializerXml& deserializer);
virtual void serialize_json(SerializerJson& serializer) const;
virtual void deserialize_json(DeserializerJson& deserializer);

int parameter;
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestEnumValue2_h__
