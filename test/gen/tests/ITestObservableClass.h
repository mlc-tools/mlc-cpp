#ifndef __mg_ITestObservableClass_h__
#define __mg_ITestObservableClass_h__

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


    class ITestObservableClass : public TestCase
    {
    public:
        ITestObservableClass();
        ~ITestObservableClass();
void execute();
bool operator ==(const ITestObservableClass& rhs) const;
bool operator !=(const ITestObservableClass& rhs) const;
 ITestObservableClass(const ITestObservableClass& rhs);
const ITestObservableClass& operator =(const ITestObservableClass& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestObservableClass_h__
