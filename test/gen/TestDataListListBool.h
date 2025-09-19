#ifndef __mg_TestDataListListBool_h__
#define __mg_TestDataListListBool_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestDataListListBool.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestDataListListBool : public ITestDataListListBool
    {
    public:
        TestDataListListBool();
        ~TestDataListListBool();
void test_foo();
bool operator ==(const TestDataListListBool& rhs) const;
bool operator !=(const TestDataListListBool& rhs) const;
 TestDataListListBool(const TestDataListListBool& rhs);
const TestDataListListBool& operator =(const TestDataListListBool& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestDataListListBool_h__
