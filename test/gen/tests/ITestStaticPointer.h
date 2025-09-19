#ifndef __mg_ITestStaticPointer_h__
#define __mg_ITestStaticPointer_h__

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

    class ITestStaticPointer : public TestCase
    {
    public:
        ITestStaticPointer();
        virtual ~ITestStaticPointer();
        virtual void test_foo() = 0;
        void execute();
        bool operator ==(const ITestStaticPointer& rhs) const;
        bool operator !=(const ITestStaticPointer& rhs) const;
        ITestStaticPointer(const ITestStaticPointer& rhs);
        const ITestStaticPointer& operator =(const ITestStaticPointer& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestStaticPointer_h__
