#ifndef __mg_ITestOuterClassInnerClass_h__
#define __mg_ITestOuterClassInnerClass_h__

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

    class ITestOuterClassInnerClass : public TestCase
    {
    public:
        ITestOuterClassInnerClass();
        virtual ~ITestOuterClassInnerClass();
        virtual void test_func() = 0;
        void execute();
        bool operator ==(const ITestOuterClassInnerClass& rhs) const;
        bool operator !=(const ITestOuterClassInnerClass& rhs) const;
        ITestOuterClassInnerClass(const ITestOuterClassInnerClass& rhs);
        const ITestOuterClassInnerClass& operator =(const ITestOuterClassInnerClass& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestOuterClassInnerClass_h__
