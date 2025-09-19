#ifndef __mg_ITestCloneObject_h__
#define __mg_ITestCloneObject_h__

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

    class ITestCloneObject : public TestCase
    {
    public:
        ITestCloneObject();
        virtual ~ITestCloneObject();
        virtual void test_foo() = 0;
        virtual void test_clone2() = 0;
        virtual void test_copy_list_with_null() = 0;
        virtual void test_copy_list_with_null2() = 0;
        virtual void test_copy_map1() = 0;
        virtual void test_copy_map2() = 0;
        virtual void test_copy_map3() = 0;
        virtual void test_copy_map4() = 0;
        virtual void test_copy_map5() = 0;
        virtual void test_copy_map6() = 0;
        virtual void test_copy_map7() = 0;
        virtual void test_copy_map8() = 0;
        void execute();
        bool operator ==(const ITestCloneObject& rhs) const;
        bool operator !=(const ITestCloneObject& rhs) const;
        ITestCloneObject(const ITestCloneObject& rhs);
        const ITestCloneObject& operator =(const ITestCloneObject& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestCloneObject_h__
