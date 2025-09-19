#ifndef __mg_DataListListBool_h__
#define __mg_DataListListBool_h__

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


    class DataListListBool
    {
    public:
        DataListListBool();
        ~DataListListBool();
void foo();
void retain();
int release();
bool operator ==(const DataListListBool& rhs) const;
bool operator !=(const DataListListBool& rhs) const;
 DataListListBool(const DataListListBool& rhs);
const DataListListBool& operator =(const DataListListBool& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

std::string name;
std::vector<std::vector<bool>> list_list_bool;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_DataListListBool_h__
