#ifndef __mg_ITestDataAdd_h__
#define __mg_ITestDataAdd_h__

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


    class ITestDataAdd : public TestCase
    {
    public:
        ITestDataAdd();
        virtual ~ITestDataAdd();
virtual void test_test() = 0;
void execute();
bool operator ==(const ITestDataAdd& rhs) const;
bool operator !=(const ITestDataAdd& rhs) const;
 ITestDataAdd(const ITestDataAdd& rhs);
const ITestDataAdd& operator =(const ITestDataAdd& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestDataAdd_h__
