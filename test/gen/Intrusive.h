#ifndef __mg_Intrusive_h__
#define __mg_Intrusive_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class Intrusive
    {
    public:
        Intrusive();
        ~Intrusive();
void add_self_to_list(std::vector<intrusive_ptr<Intrusive>>& list);
void remove_self_from_list(std::vector<intrusive_ptr<Intrusive>>& list);
void retain();
int release();
bool operator ==(const Intrusive& rhs) const;
bool operator !=(const Intrusive& rhs) const;
 Intrusive(const Intrusive& rhs);
const Intrusive& operator =(const Intrusive& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_Intrusive_h__
