#ifndef __mg_TestFooEnumTestParent_h__
#define __mg_TestFooEnumTestParent_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "EnumWithIntValue.h"
#include "FooEnum.h"
#include "tests/ITestFooEnumTestParent.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestFooEnumTestParent : public ITestFooEnumTestParent
    {
    public:
        TestFooEnumTestParent();
        ~TestFooEnumTestParent();
        void test_dummy();
        void test_compare();
        void test_initialize();
        void test_enum_with_int_values();
        bool operator ==(const TestFooEnumTestParent& rhs) const;
        bool operator !=(const TestFooEnumTestParent& rhs) const;
        TestFooEnumTestParent(const TestFooEnumTestParent& rhs);
        const TestFooEnumTestParent& operator =(const TestFooEnumTestParent& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        FooEnum enumValue;
        FooEnum enumFoo;
        FooEnum enumBar;
        EnumWithIntValue enum_int1;
        EnumWithIntValue enum_int2;
        EnumWithIntValue enum_int3;
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestFooEnumTestParent_h__
