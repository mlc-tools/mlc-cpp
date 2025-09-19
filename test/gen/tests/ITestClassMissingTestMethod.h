#ifndef __mg_ITestClassMissingTestMethod_h__
#define __mg_ITestClassMissingTestMethod_h__

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

    class ITestClassMissingTestMethod : public TestCase
    {
    public:
        ITestClassMissingTestMethod();
        virtual ~ITestClassMissingTestMethod();
        virtual void test_foo() = 0;
        void test_bar();
        void execute();
        bool operator ==(const ITestClassMissingTestMethod& rhs) const;
        bool operator !=(const ITestClassMissingTestMethod& rhs) const;
        ITestClassMissingTestMethod(const ITestClassMissingTestMethod& rhs);
        const ITestClassMissingTestMethod& operator =(const ITestClassMissingTestMethod& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestClassMissingTestMethod_h__
