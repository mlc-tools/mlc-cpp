#ifndef __mg_TestCloneObject_h__
#define __mg_TestCloneObject_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../tests/ITestCloneObject.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestCloneObject : public ITestCloneObject
    {
    public:
        TestCloneObject();
        ~TestCloneObject();
        void test_foo();
        void test_clone2();
        void test_copy_list_with_null();
        void test_copy_list_with_null2();
        void test_copy_map1();
        void test_copy_map2();
        void test_copy_map3();
        void test_copy_map4();
        void test_copy_map5();
        void test_copy_map6();
        void test_copy_map7();
        void test_copy_map8();
        bool operator ==(const TestCloneObject& rhs) const;
        bool operator !=(const TestCloneObject& rhs) const;
        TestCloneObject(const TestCloneObject& rhs);
        const TestCloneObject& operator =(const TestCloneObject& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestCloneObject_h__
