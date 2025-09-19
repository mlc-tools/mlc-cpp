#ifndef __mg_ITestTemplateMethod_h__
#define __mg_ITestTemplateMethod_h__

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

    class ITestTemplateMethod : public TestCase
    {
    public:
        ITestTemplateMethod();
        virtual ~ITestTemplateMethod();
        virtual void test_get() = 0;
        virtual void test_default() = 0;
        void execute();
        bool operator ==(const ITestTemplateMethod& rhs) const;
        bool operator !=(const ITestTemplateMethod& rhs) const;
        ITestTemplateMethod(const ITestTemplateMethod& rhs);
        const ITestTemplateMethod& operator =(const ITestTemplateMethod& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestTemplateMethod_h__
