#ifndef __mg_NullishOperator_h__
#define __mg_NullishOperator_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Point.h"
#include <atomic>
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class NullishOperator
    {
    public:
        NullishOperator();
        ~NullishOperator();
void dummy_function();
void retain();
int release();
bool operator ==(const NullishOperator& rhs) const;
bool operator !=(const NullishOperator& rhs) const;
 NullishOperator(const NullishOperator& rhs);
const NullishOperator& operator =(const NullishOperator& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

intrusive_ptr<Point> point_a;
intrusive_ptr<Point> point_b;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_NullishOperator_h__
