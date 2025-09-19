#ifndef __mg_TestDataDict_h__
#define __mg_TestDataDict_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../Resource.h"
#include "../tests/ITestDataDict.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestDataDict : public ITestDataDict
    {
    public:
        TestDataDict();
        ~TestDataDict();
        void test_resources_in_map();
        void test_resources_def_value();
        void test_skills();
        bool operator ==(const TestDataDict& rhs) const;
        bool operator !=(const TestDataDict& rhs) const;
        TestDataDict(const TestDataDict& rhs);
        const TestDataDict& operator =(const TestDataDict& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::map<Resource, int> resources;
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestDataDict_h__
