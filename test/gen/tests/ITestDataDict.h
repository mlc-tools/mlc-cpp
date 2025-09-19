#ifndef __mg_ITestDataDict_h__
#define __mg_ITestDataDict_h__

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


    class ITestDataDict : public TestCase
    {
    public:
        ITestDataDict();
        virtual ~ITestDataDict();
virtual void test_resources_in_map() = 0;
virtual void test_resources_def_value() = 0;
virtual void test_skills() = 0;
void execute();
bool operator ==(const ITestDataDict& rhs) const;
bool operator !=(const ITestDataDict& rhs) const;
 ITestDataDict(const ITestDataDict& rhs);
const ITestDataDict& operator =(const ITestDataDict& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestDataDict_h__
