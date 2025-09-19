#ifndef __mg_TestTemplateMethod_h__
#define __mg_TestTemplateMethod_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestTemplateMethod.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestTemplateMethod : public ITestTemplateMethod
    {
    public:
        TestTemplateMethod();
        ~TestTemplateMethod();
        void test_get();
        void test_default();
        bool operator ==(const TestTemplateMethod& rhs) const;
        bool operator !=(const TestTemplateMethod& rhs) const;
        TestTemplateMethod(const TestTemplateMethod& rhs);
        const TestTemplateMethod& operator =(const TestTemplateMethod& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestTemplateMethod_h__
