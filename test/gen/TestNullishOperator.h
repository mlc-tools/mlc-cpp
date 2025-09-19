#ifndef __mg_TestNullishOperator_h__
#define __mg_TestNullishOperator_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestNullishOperator.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestNullishOperator : public ITestNullishOperator
    {
    public:
        TestNullishOperator();
        ~TestNullishOperator();
void test_dummy_function();
void test_nullish_coalescing_operator();
bool operator ==(const TestNullishOperator& rhs) const;
bool operator !=(const TestNullishOperator& rhs) const;
 TestNullishOperator(const TestNullishOperator& rhs);
const TestNullishOperator& operator =(const TestNullishOperator& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestNullishOperator_h__
