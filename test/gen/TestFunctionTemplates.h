#ifndef __mg_TestFunctionTemplates_h__
#define __mg_TestFunctionTemplates_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestFunctionTemplates.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestFunctionTemplates : public ITestFunctionTemplates
    {
    public:
        TestFunctionTemplates();
        ~TestFunctionTemplates();
void test_add();
void test_add_strings();
void test_use_other_class();
bool operator ==(const TestFunctionTemplates& rhs) const;
bool operator !=(const TestFunctionTemplates& rhs) const;
 TestFunctionTemplates(const TestFunctionTemplates& rhs);
const TestFunctionTemplates& operator =(const TestFunctionTemplates& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestFunctionTemplates_h__
