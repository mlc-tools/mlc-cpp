#ifndef __mg_TestDataComplexMap_h__
#define __mg_TestDataComplexMap_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "tests/ITestDataComplexMap.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestDataComplexMap : public ITestDataComplexMap
    {
    public:
        TestDataComplexMap();
        ~TestDataComplexMap();
        void test_foo();
        void test_deserialize();
        bool operator ==(const TestDataComplexMap& rhs) const;
        bool operator !=(const TestDataComplexMap& rhs) const;
        TestDataComplexMap(const TestDataComplexMap& rhs);
        const TestDataComplexMap& operator =(const TestDataComplexMap& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestDataComplexMap_h__
