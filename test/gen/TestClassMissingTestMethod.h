#ifndef __mg_TestClassMissingTestMethod_h__
#define __mg_TestClassMissingTestMethod_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestClassMissingTestMethod.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestClassMissingTestMethod : public ITestClassMissingTestMethod
    {
    public:
        TestClassMissingTestMethod();
        ~TestClassMissingTestMethod();
        void test_foo();
        bool operator ==(const TestClassMissingTestMethod& rhs) const;
        bool operator !=(const TestClassMissingTestMethod& rhs) const;
        TestClassMissingTestMethod(const TestClassMissingTestMethod& rhs);
        const TestClassMissingTestMethod& operator =(const TestClassMissingTestMethod& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestClassMissingTestMethod_h__
