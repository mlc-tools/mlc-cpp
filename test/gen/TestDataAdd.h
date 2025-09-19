#ifndef __mg_TestDataAdd_h__
#define __mg_TestDataAdd_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestDataAdd.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestDataAdd : public ITestDataAdd
    {
    public:
        TestDataAdd();
        ~TestDataAdd();
void test_test();
bool operator ==(const TestDataAdd& rhs) const;
bool operator !=(const TestDataAdd& rhs) const;
 TestDataAdd(const TestDataAdd& rhs);
const TestDataAdd& operator =(const TestDataAdd& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestDataAdd_h__
