#ifndef __mg_ITestCompareTestWithoutPointer_h__
#define __mg_ITestCompareTestWithoutPointer_h__

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

    class ITestCompareTestWithoutPointer : public TestCase
    {
    public:
        ITestCompareTestWithoutPointer();
        virtual ~ITestCompareTestWithoutPointer();
        virtual void test_test() = 0;
        virtual void test_operator_copy() = 0;
        virtual void test_without_pointer_compare() = 0;
        virtual void test_with_pointer_compare() = 0;
        void execute();
        bool operator ==(const ITestCompareTestWithoutPointer& rhs) const;
        bool operator !=(const ITestCompareTestWithoutPointer& rhs) const;
        ITestCompareTestWithoutPointer(const ITestCompareTestWithoutPointer& rhs);
        const ITestCompareTestWithoutPointer& operator =(const ITestCompareTestWithoutPointer& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestCompareTestWithoutPointer_h__
