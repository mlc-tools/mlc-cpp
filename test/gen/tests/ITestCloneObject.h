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
