#ifndef __mg_LadderLevels_h__
#define __mg_LadderLevels_h__

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


    class LadderLevels
    {
    public:
        LadderLevels();
        ~LadderLevels();
void retain();
int release();
bool operator ==(const LadderLevels& rhs) const;
bool operator !=(const LadderLevels& rhs) const;
 LadderLevels(const LadderLevels& rhs);
const LadderLevels& operator =(const LadderLevels& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

std::string name;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_LadderLevels_h__
