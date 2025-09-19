#ifndef __mg_ITestOuterClass_h__
#define __mg_ITestOuterClass_h__

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

    class ITestOuterClass : public TestCase
    {
    public:
        ITestOuterClass();
        virtual ~ITestOuterClass();
        virtual void test_func() = 0;
        virtual void test_func2() = 0;
        void execute();
        bool operator ==(const ITestOuterClass& rhs) const;
        bool operator !=(const ITestOuterClass& rhs) const;
        ITestOuterClass(const ITestOuterClass& rhs);
        const ITestOuterClass& operator =(const ITestOuterClass& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestOuterClass_h__
