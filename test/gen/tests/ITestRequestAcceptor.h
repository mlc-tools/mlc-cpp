#ifndef __mg_ITestRequestAcceptor_h__
#define __mg_ITestRequestAcceptor_h__

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


    class ITestRequestAcceptor : public TestCase
    {
    public:
        ITestRequestAcceptor();
        virtual ~ITestRequestAcceptor();
virtual void test_visitor() = 0;
void execute();
bool operator ==(const ITestRequestAcceptor& rhs) const;
bool operator !=(const ITestRequestAcceptor& rhs) const;
 ITestRequestAcceptor(const ITestRequestAcceptor& rhs);
const ITestRequestAcceptor& operator =(const ITestRequestAcceptor& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestRequestAcceptor_h__
