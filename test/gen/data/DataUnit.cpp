#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../EnumWithIntValue.h"
#include "../SkillBase.h"
#include "../UnitType.h"
#include "../tests/Logger.h"
#include "DataNonExist.h"
#include "DataUnit.h"
#include "VisualUnit.h"
#include <atomic>
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataUnit::TYPE("DataUnit");

    
    DataUnit::DataUnit()
    : name()
, unit_type()
, visual()
, link_to_data(nullptr)
, all_units()
, map_units()
, map_units_data()
, enum_with_int_value()
, int64_value(0)
, int64_value_without_default_value(0)
, uint64_value(0)
, uns_int_value(0)
, double_value(0)
, double_value_without_default_value(0.0)
, float_value(0)
, float_value_without_default_value(0.0f)
, skill(nullptr)
, empty()
, convert_float_to_int(0)
, _reference_counter(1)
{
    
    }

    DataUnit::~DataUnit(){}


bool DataUnit::tests(Logger* logger)
{

	    auto data = DataStorage::shared().get<DataUnit>("unitname1");

	    auto result = true;
	    result = DataUnit::test_0(data) && result;
	    result = DataUnit::test_links(logger) && result;
	    result = DataUnit::test_enums(logger) && result;
	    result = DataUnit::test_list_links(logger) && result;
	    result = DataUnit::test_map_links(logger) && result;
	    result = DataUnit::test_map_data_links(logger) && result;
	    result = DataUnit::test_convert_float_to_int(logger) && result;

	    return result;
	
}

bool DataUnit::test_0(const DataUnit* data)
{

	    return true;
	
}

bool DataUnit::test_1(Logger* logger)
{

	    auto result = true;
	    auto units = DataStorage::shared().get_units();
        for (auto&& pair : units)
{
    auto& name = pair.first;
    auto& unit = pair.second;
    (void)name; // don't generate 'Unused variable' warning
    (void)unit; // don't generate 'Unused variable' warning
            result = result && name == unit.name;
        }
        return result;
	
}

bool DataUnit::test_links(Logger* logger)
{

        bool result = true;

        auto units = DataStorage::shared().get_units_keys();
        result = (2 == list_size(units)) && result;

        auto unit1 = DataStorage::shared().get<DataUnit>("unitname1");
        auto unit2 = DataStorage::shared().get<DataUnit>("unitname2");

        result = result && unit1->link_to_data->name == unit2->name;
        result = result && unit2->link_to_data->name == unit1->name;

        result = result && map_size(unit2->link_to_data->map_units) == map_size(unit1->map_units);
        result = result && map_size(unit1->link_to_data->map_units) == map_size(unit2->map_units);

        result = result && unit1 != unit2;
        return result;
	
}

bool DataUnit::test_enums(Logger* logger)
{

        bool result = true;

        auto unit1 = DataStorage::shared().get<DataUnit>("unitname1");
        auto unit2 = DataStorage::shared().get<DataUnit>("unitname2");

        result = result && unit1->unit_type == UnitType::attack;
        result = result && unit1->unit_type != UnitType::defend;
        result = result && unit1->unit_type != UnitType::support;

        result = result && unit2->unit_type == UnitType::defend;
        result = result && unit2->unit_type != UnitType::attack;
        result = result && unit2->unit_type != UnitType::support;
        return result;
    
}

bool DataUnit::test_list_links(Logger* logger)
{

        bool result = true;

        auto unit1 = DataStorage::shared().get<DataUnit>("unitname1");
        auto unit2 = DataStorage::shared().get<DataUnit>("unitname2");

        result = result && list_size(unit1->all_units) == 2;
        result = result && list_size(unit2->all_units) == 2;
        for (auto unit : unit1->all_units)
        {
            result = result && (unit->name == unit1->name || unit->name == unit2->name);
        }
        for (auto unit : unit2->all_units)
        {
            result = result && (unit->name == unit1->name || unit->name == unit2->name);
        }

        return result;
    
}

bool DataUnit::test_map_links(Logger* logger)
{

        bool result = true;

        auto unit1 = DataStorage::shared().get<DataUnit>("unitname1");
        auto unit2 = DataStorage::shared().get<DataUnit>("unitname2");

        result = result && map_size(unit1->map_units) == 2;
        result = result && map_size(unit2->map_units) == 2;
        for (auto&& pair : unit1->map_units)
{
    auto& name = pair.first;
    auto& count = pair.second;
    (void)name; // don't generate 'Unused variable' warning
    (void)count; // don't generate 'Unused variable' warning
            result = result && (name == unit1->name || name == unit2->name);
            result = result && (count == 1 || count == 2);
        }
        for (auto&& pair : unit2->map_units)
{
    auto& name = pair.first;
    auto& count = pair.second;
    (void)name; // don't generate 'Unused variable' warning
    (void)count; // don't generate 'Unused variable' warning
            result = result && (name == unit1->name || name == unit2->name);
            result = result && (count == 1 || count == 2);
        }
        return result;
    
}

bool DataUnit::test_map_data_links(Logger* logger)
{

        bool result = true;

        auto unit1 = DataStorage::shared().get<DataUnit>("unitname1");
        auto unit2 = DataStorage::shared().get<DataUnit>("unitname2");

        result = result && map_size(unit1->map_units_data) == 2;
        result = result && map_size(unit2->map_units_data) == 2;
        for (auto&& pair : unit1->map_units_data)
{
    auto& data = pair.first;
    auto& count = pair.second;
    (void)data; // don't generate 'Unused variable' warning
    (void)count; // don't generate 'Unused variable' warning
            result = result && (data == unit1 || data == unit2);
            result = result && (count == 1 || count == 2);
        }
        for (auto&& pair : unit2->map_units_data)
{
    auto& data = pair.first;
    auto& count = pair.second;
    (void)data; // don't generate 'Unused variable' warning
    (void)count; // don't generate 'Unused variable' warning
            result = result && (data == unit1 || data == unit2);
            result = result && (count == 1 || count == 2);
        }

        return result;
    
}

bool DataUnit::test_convert_float_to_int(Logger* logger)
{

        bool result = true;
        auto unit1 = DataStorage::shared().get<DataUnit>("unitname1");
        auto unit2 = DataStorage::shared().get<DataUnit>("unitname2");

        result = result && unit1->convert_float_to_int == 1;
        result = result && unit2->convert_float_to_int == 1;
        if(!result)
        {
            logger->message("Fail: DataUnit test_convert_float_to_int");
            logger->message("      unit1 value = " + toStr(unit1->convert_float_to_int));
            logger->message("      unit2 value = " + toStr(unit2->convert_float_to_int));
        }
        return result;
    
}

void DataUnit::retain()
{
++this->_reference_counter;
}

int DataUnit::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool DataUnit::operator ==(const DataUnit& rhs) const
{
bool result = true;
result = result && this->name == rhs.name;
result = result && this->unit_type == rhs.unit_type;
result = result && this->visual == rhs.visual;
result = result && ((this->link_to_data == rhs.link_to_data) || (this->link_to_data != nullptr && rhs.link_to_data != nullptr && *this->link_to_data == *rhs.link_to_data));
result = result && this->all_units == rhs.all_units;
result = result && this->map_units == rhs.map_units;
result = result && this->map_units_data == rhs.map_units_data;
result = result && this->enum_with_int_value == rhs.enum_with_int_value;
result = result && this->int64_value == rhs.int64_value;
result = result && this->int64_value_without_default_value == rhs.int64_value_without_default_value;
result = result && this->uint64_value == rhs.uint64_value;
result = result && this->uns_int_value == rhs.uns_int_value;
result = result && this->double_value == rhs.double_value;
result = result && this->double_value_without_default_value == rhs.double_value_without_default_value;
result = result && this->float_value == rhs.float_value;
result = result && this->float_value_without_default_value == rhs.float_value_without_default_value;
result = result && ((this->skill == rhs.skill) || (this->skill != nullptr && rhs.skill != nullptr && *this->skill == *rhs.skill));
result = result && ((this->empty == rhs.empty) || (this->empty != nullptr && rhs.empty != nullptr && *this->empty == *rhs.empty));
result = result && this->convert_float_to_int == rhs.convert_float_to_int;
return result;
}

bool DataUnit::operator !=(const DataUnit& rhs) const
{

return !(*this == rhs);
}

 DataUnit::DataUnit(const DataUnit& rhs)
{

this->operator=(rhs);
}

const DataUnit& DataUnit::operator =(const DataUnit& rhs)
{

this->name = rhs.name;
this->unit_type = rhs.unit_type;
this->visual = rhs.visual;
this->link_to_data = rhs.link_to_data;
this->all_units = rhs.all_units;
this->map_units = rhs.map_units;
this->map_units_data = rhs.map_units_data;
this->enum_with_int_value = rhs.enum_with_int_value;
this->int64_value = rhs.int64_value;
this->int64_value_without_default_value = rhs.int64_value_without_default_value;
this->uint64_value = rhs.uint64_value;
this->uns_int_value = rhs.uns_int_value;
this->double_value = rhs.double_value;
this->double_value_without_default_value = rhs.double_value_without_default_value;
this->float_value = rhs.float_value;
this->float_value_without_default_value = rhs.float_value_without_default_value;
this->skill = rhs.skill;
this->empty = rhs.empty;
this->convert_float_to_int = rhs.convert_float_to_int;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string DataUnit::get_type() const
{
return DataUnit::TYPE;
}

void DataUnit::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(name, "name", std::string());
serializer.serialize(unit_type, "unit_type");
serializer.serialize(visual, "visual");
serializer.serialize(link_to_data, "link_to_data");
serializer.serialize(all_units, "all_units");
serializer.serialize(map_units, "map_units");
serializer.serialize(map_units_data, "map_units_data");
serializer.serialize(enum_with_int_value, "enum_with_int_value");
serializer.serialize(int64_value, "int64_value", int64_t(0));
serializer.serialize(int64_value_without_default_value, "int64_value_without_default_value", int64_t(0));
serializer.serialize(uint64_value, "uint64_value", uint64_t(0));
serializer.serialize(uns_int_value, "uns_int_value", unsigned(0));
serializer.serialize(double_value, "double_value", double(0));
serializer.serialize(double_value_without_default_value, "double_value_without_default_value", double(0.0));
serializer.serialize(float_value, "float_value", float(0));
serializer.serialize(float_value_without_default_value, "float_value_without_default_value", float(0.0f));
serializer.serialize(skill, "skill");
serializer.serialize(empty, "empty");
serializer.serialize(convert_float_to_int, "convert_float_to_int", int(0));
}

void DataUnit::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(name, "name", std::string());
deserializer.deserialize(unit_type, "unit_type");
deserializer.deserialize(visual, "visual");
deserializer.deserialize(link_to_data, "link_to_data");
deserializer.deserialize(all_units, "all_units");
deserializer.deserialize(map_units, "map_units");
deserializer.deserialize(map_units_data, "map_units_data");
deserializer.deserialize(enum_with_int_value, "enum_with_int_value");
deserializer.deserialize(int64_value, "int64_value", int64_t(0));
deserializer.deserialize(int64_value_without_default_value, "int64_value_without_default_value", int64_t(0));
deserializer.deserialize(uint64_value, "uint64_value", uint64_t(0));
deserializer.deserialize(uns_int_value, "uns_int_value", unsigned(0));
deserializer.deserialize(double_value, "double_value", double(0));
deserializer.deserialize(double_value_without_default_value, "double_value_without_default_value", double(0.0));
deserializer.deserialize(float_value, "float_value", float(0));
deserializer.deserialize(float_value_without_default_value, "float_value_without_default_value", float(0.0f));
deserializer.deserialize(skill, "skill");
deserializer.deserialize(empty, "empty");
deserializer.deserialize(convert_float_to_int, "convert_float_to_int", int(0));
}

void DataUnit::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(name, "name", std::string());
serializer.serialize(unit_type, "unit_type");
serializer.serialize(visual, "visual");
serializer.serialize(link_to_data, "link_to_data");
serializer.serialize(all_units, "all_units");
serializer.serialize(map_units, "map_units");
serializer.serialize(map_units_data, "map_units_data");
serializer.serialize(enum_with_int_value, "enum_with_int_value");
serializer.serialize(int64_value, "int64_value", int64_t(0));
serializer.serialize(int64_value_without_default_value, "int64_value_without_default_value", int64_t(0));
serializer.serialize(uint64_value, "uint64_value", uint64_t(0));
serializer.serialize(uns_int_value, "uns_int_value", unsigned(0));
serializer.serialize(double_value, "double_value", double(0));
serializer.serialize(double_value_without_default_value, "double_value_without_default_value", double(0.0));
serializer.serialize(float_value, "float_value", float(0));
serializer.serialize(float_value_without_default_value, "float_value_without_default_value", float(0.0f));
serializer.serialize(skill, "skill");
serializer.serialize(empty, "empty");
serializer.serialize(convert_float_to_int, "convert_float_to_int", int(0));
}

void DataUnit::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(name, "name", std::string());
deserializer.deserialize(unit_type, "unit_type");
deserializer.deserialize(visual, "visual");
deserializer.deserialize(link_to_data, "link_to_data");
deserializer.deserialize(all_units, "all_units");
deserializer.deserialize(map_units, "map_units");
deserializer.deserialize(map_units_data, "map_units_data");
deserializer.deserialize(enum_with_int_value, "enum_with_int_value");
deserializer.deserialize(int64_value, "int64_value", int64_t(0));
deserializer.deserialize(int64_value_without_default_value, "int64_value_without_default_value", int64_t(0));
deserializer.deserialize(uint64_value, "uint64_value", uint64_t(0));
deserializer.deserialize(uns_int_value, "uns_int_value", unsigned(0));
deserializer.deserialize(double_value, "double_value", double(0));
deserializer.deserialize(double_value_without_default_value, "double_value_without_default_value", double(0.0));
deserializer.deserialize(float_value, "float_value", float(0));
deserializer.deserialize(float_value_without_default_value, "float_value_without_default_value", float(0.0f));
deserializer.deserialize(skill, "skill");
deserializer.deserialize(empty, "empty");
deserializer.deserialize(convert_float_to_int, "convert_float_to_int", int(0));
}

} // namespace mg
