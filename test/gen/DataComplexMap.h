#ifndef __mg_DataComplexMap_h__
#define __mg_DataComplexMap_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <map>
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class DataComplexMap
    {
    public:
        DataComplexMap();
        ~DataComplexMap();
void foo();
private:
void bar_private();
public:
void retain();
int release();
bool operator ==(const DataComplexMap& rhs) const;
bool operator !=(const DataComplexMap& rhs) const;
 DataComplexMap(const DataComplexMap& rhs);
const DataComplexMap& operator =(const DataComplexMap& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

std::string name;
std::map<int, std::vector<float>> complex_map;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_DataComplexMap_h__
