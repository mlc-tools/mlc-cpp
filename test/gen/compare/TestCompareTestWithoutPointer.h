#ifndef __mg_TestCompareTestWithoutPointer_h__
#define __mg_TestCompareTestWithoutPointer_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../tests/ITestCompareTestWithoutPointer.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestCompareTestWithoutPointer : public ITestCompareTestWithoutPointer
    {
    public:
        TestCompareTestWithoutPointer();
        ~TestCompareTestWithoutPointer();
void test_test();
void test_operator_copy();
void test_without_pointer_compare();
void test_with_pointer_compare();
bool operator ==(const TestCompareTestWithoutPointer& rhs) const;
bool operator !=(const TestCompareTestWithoutPointer& rhs) const;
 TestCompareTestWithoutPointer(const TestCompareTestWithoutPointer& rhs);
const TestCompareTestWithoutPointer& operator =(const TestCompareTestWithoutPointer& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestCompareTestWithoutPointer_h__
