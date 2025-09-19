#ifndef __mg_CompareTestWithoutPointer_h__
#define __mg_CompareTestWithoutPointer_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "CompareTestBase.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class CompareTestWithoutPointer : public CompareTestBase
    {
    public:
        CompareTestWithoutPointer();
        ~CompareTestWithoutPointer();
void test();
bool operator ==(const CompareTestWithoutPointer& rhs) const;
bool operator !=(const CompareTestWithoutPointer& rhs) const;
 CompareTestWithoutPointer(const CompareTestWithoutPointer& rhs);
const CompareTestWithoutPointer& operator =(const CompareTestWithoutPointer& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

int a;
std::string b;
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_CompareTestWithoutPointer_h__
