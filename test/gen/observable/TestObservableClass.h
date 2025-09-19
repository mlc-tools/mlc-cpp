#ifndef __mg_TestObservableClass_h__
#define __mg_TestObservableClass_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../tests/ITestObservableClass.h"
#include "ObservableClass.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestObservableClass : public ITestObservableClass
    {
    public:
        TestObservableClass();
        ~TestObservableClass();
        void test_1();
        void test_notify_with_unsubscribe();
        void test_listen_with_two();
        void test_with_arg_int();
        void test_with_arg_int_two();
        void test_with_data();
        void test_with_pointer();
        void listen();
        void listen_with_remove();
        bool operator ==(const TestObservableClass& rhs) const;
        bool operator !=(const TestObservableClass& rhs) const;
        TestObservableClass(const TestObservableClass& rhs);
        const TestObservableClass& operator =(const TestObservableClass& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int value;
        ObservableClass test;
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestObservableClass_h__
