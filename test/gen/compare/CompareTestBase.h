#ifndef __mg_CompareTestBase_h__
#define __mg_CompareTestBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class CompareTestBase
    {
    public:
        CompareTestBase();
        ~CompareTestBase();
void retain();
int release();
bool operator ==(const CompareTestBase& rhs) const;
bool operator !=(const CompareTestBase& rhs) const;
 CompareTestBase(const CompareTestBase& rhs);
const CompareTestBase& operator =(const CompareTestBase& rhs);
virtual std::string get_type() const;
virtual void serialize_xml(SerializerXml& serializer) const;
virtual void deserialize_xml(DeserializerXml& deserializer);
virtual void serialize_json(SerializerJson& serializer) const;
virtual void deserialize_json(DeserializerJson& deserializer);

int id;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_CompareTestBase_h__
