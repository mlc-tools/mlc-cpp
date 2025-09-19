#ifndef __mg_TestStaticPointer_h__
#define __mg_TestStaticPointer_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestStaticPointer.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestStaticPointer : public ITestStaticPointer
    {
    public:
        TestStaticPointer();
        ~TestStaticPointer();
void test_foo();
bool operator ==(const TestStaticPointer& rhs) const;
bool operator !=(const TestStaticPointer& rhs) const;
 TestStaticPointer(const TestStaticPointer& rhs);
const TestStaticPointer& operator =(const TestStaticPointer& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestStaticPointer_h__
