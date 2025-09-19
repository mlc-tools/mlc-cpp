#ifndef __mg_TestSerializeTest_h__
#define __mg_TestSerializeTest_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestSerializeTest.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestSerializeTest : public ITestSerializeTest
    {
    public:
        TestSerializeTest();
        ~TestSerializeTest();
        void test_serialize_python();
        bool operator ==(const TestSerializeTest& rhs) const;
        bool operator !=(const TestSerializeTest& rhs) const;
        TestSerializeTest(const TestSerializeTest& rhs);
        const TestSerializeTest& operator =(const TestSerializeTest& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestSerializeTest_h__
