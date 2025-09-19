#ifndef __mg_ITestSerializeTest_h__
#define __mg_ITestSerializeTest_h__

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

    class ITestSerializeTest : public TestCase
    {
    public:
        ITestSerializeTest();
        virtual ~ITestSerializeTest();
        virtual void test_serialize_python() = 0;
        void execute();
        bool operator ==(const ITestSerializeTest& rhs) const;
        bool operator !=(const ITestSerializeTest& rhs) const;
        ITestSerializeTest(const ITestSerializeTest& rhs);
        const ITestSerializeTest& operator =(const ITestSerializeTest& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestSerializeTest_h__
