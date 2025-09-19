#ifndef __mg_TestConstructableClass_h__
#define __mg_TestConstructableClass_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestConstructableClass.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestConstructableClass : public ITestConstructableClass
    {
    public:
        TestConstructableClass();
        ~TestConstructableClass();
        void test_constructor();
        void test_constructor_with_args();
        void test_constructor_with_args_2();
        bool operator ==(const TestConstructableClass& rhs) const;
        bool operator !=(const TestConstructableClass& rhs) const;
        TestConstructableClass(const TestConstructableClass& rhs);
        const TestConstructableClass& operator =(const TestConstructableClass& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestConstructableClass_h__
