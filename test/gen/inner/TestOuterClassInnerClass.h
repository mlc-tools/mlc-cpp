#ifndef __mg_TestOuterClassInnerClass_h__
#define __mg_TestOuterClassInnerClass_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../tests/ITestOuterClassInnerClass.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestOuterClassInnerClass : public ITestOuterClassInnerClass
    {
    public:
        TestOuterClassInnerClass();
        ~TestOuterClassInnerClass();
void test_func();
bool operator ==(const TestOuterClassInnerClass& rhs) const;
bool operator !=(const TestOuterClassInnerClass& rhs) const;
 TestOuterClassInnerClass(const TestOuterClassInnerClass& rhs);
const TestOuterClassInnerClass& operator =(const TestOuterClassInnerClass& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestOuterClassInnerClass_h__
