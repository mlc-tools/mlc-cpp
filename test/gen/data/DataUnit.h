#ifndef __mg_DataUnit_h__
#define __mg_DataUnit_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../EnumWithIntValue.h"
#include "../SkillBase.h"
#include "../UnitType.h"
#include "DataNonExist.h"
#include "VisualUnit.h"
#include <atomic>
#include <map>
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
class Logger;

    class DataUnit
    {
    public:
        DataUnit();
        ~DataUnit();
static bool tests(Logger* logger);
static bool test_0(const DataUnit* data);
static bool test_1(Logger* logger);
static bool test_links(Logger* logger);
static bool test_enums(Logger* logger);
static bool test_list_links(Logger* logger);
static bool test_map_links(Logger* logger);
static bool test_map_data_links(Logger* logger);
static bool test_convert_float_to_int(Logger* logger);
void retain();
int release();
bool operator ==(const DataUnit& rhs) const;
bool operator !=(const DataUnit& rhs) const;
 DataUnit(const DataUnit& rhs);
const DataUnit& operator =(const DataUnit& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

std::string name;
UnitType unit_type;
VisualUnit visual;
const DataUnit* link_to_data;
std::vector<const DataUnit*> all_units;
std::map<std::string, int> map_units;
std::map<const DataUnit*, int> map_units_data;
EnumWithIntValue enum_with_int_value;
int64_t int64_value;
int64_t int64_value_without_default_value;
uint64_t uint64_value;
unsigned uns_int_value;
double double_value;
double double_value_without_default_value;
float float_value;
float float_value_without_default_value;
intrusive_ptr<SkillBase> skill;
const DataNonExist* empty;
int convert_float_to_int;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_DataUnit_h__
