#ifndef __mg_ITestDataComplexMap_h__
#define __mg_ITestDataComplexMap_h__

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


    class ITestDataComplexMap : public TestCase
    {
    public:
        ITestDataComplexMap();
        virtual ~ITestDataComplexMap();
virtual void test_foo() = 0;
void execute();
bool operator ==(const ITestDataComplexMap& rhs) const;
bool operator !=(const ITestDataComplexMap& rhs) const;
 ITestDataComplexMap(const ITestDataComplexMap& rhs);
const ITestDataComplexMap& operator =(const ITestDataComplexMap& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestDataComplexMap_h__
