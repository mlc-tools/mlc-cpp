#ifndef __mg_ITestFooEnumTestParent_h__
#define __mg_ITestFooEnumTestParent_h__

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


    class ITestFooEnumTestParent : public TestCase
    {
    public:
        ITestFooEnumTestParent();
        virtual ~ITestFooEnumTestParent();
virtual void test_dummy() = 0;
void execute();
bool operator ==(const ITestFooEnumTestParent& rhs) const;
bool operator !=(const ITestFooEnumTestParent& rhs) const;
 ITestFooEnumTestParent(const ITestFooEnumTestParent& rhs);
const ITestFooEnumTestParent& operator =(const ITestFooEnumTestParent& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestFooEnumTestParent_h__
