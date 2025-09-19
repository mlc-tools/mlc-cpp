#ifndef __mg_ITestDataListListBool_h__
#define __mg_ITestDataListListBool_h__

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


    class ITestDataListListBool : public TestCase
    {
    public:
        ITestDataListListBool();
        virtual ~ITestDataListListBool();
virtual void test_foo() = 0;
void execute();
bool operator ==(const ITestDataListListBool& rhs) const;
bool operator !=(const ITestDataListListBool& rhs) const;
 ITestDataListListBool(const ITestDataListListBool& rhs);
const ITestDataListListBool& operator =(const ITestDataListListBool& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestDataListListBool_h__
