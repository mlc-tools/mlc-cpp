#ifndef __mg_ITestNullishOperator_h__
#define __mg_ITestNullishOperator_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "TestCase.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class ITestNullishOperator : public TestCase
    {
    public:
        ITestNullishOperator();
        virtual ~ITestNullishOperator();
virtual void test_dummy_function() = 0;
virtual void test_nullish_coalescing_operator() = 0;
void execute();
bool operator ==(const ITestNullishOperator& rhs) const;
bool operator !=(const ITestNullishOperator& rhs) const;
 ITestNullishOperator(const ITestNullishOperator& rhs);
const ITestNullishOperator& operator =(const ITestNullishOperator& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestNullishOperator_h__
