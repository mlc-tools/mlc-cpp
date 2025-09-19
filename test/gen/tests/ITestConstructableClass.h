#ifndef __mg_ITestConstructableClass_h__
#define __mg_ITestConstructableClass_h__

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


    class ITestConstructableClass : public TestCase
    {
    public:
        ITestConstructableClass();
        virtual ~ITestConstructableClass();
virtual void test_constructor() = 0;
virtual void test_constructor_with_args() = 0;
virtual void test_constructor_with_args_2() = 0;
void execute();
bool operator ==(const ITestConstructableClass& rhs) const;
bool operator !=(const ITestConstructableClass& rhs) const;
 ITestConstructableClass(const ITestConstructableClass& rhs);
const ITestConstructableClass& operator =(const ITestConstructableClass& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestConstructableClass_h__
