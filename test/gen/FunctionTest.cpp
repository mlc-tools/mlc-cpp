#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataStorage.h"
#include "FunctionTest.h"
#include "TestToFor.h"
#include "TestUser1.h"
#include "data/DataUnit.h"
#include "tests/Logger.h"
#include <map>
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string FunctionTest::TYPE("FunctionTest");

    FunctionTest::FunctionTest()
    : initialized(false)
    , data_unit(nullptr)
    , list()
    , map()
    , _reference_counter(1)
    {

    }

    FunctionTest::~FunctionTest(){}

    void FunctionTest::initialize()
    {

        this->initialized = true;
        this->data_unit = DataStorage::shared().get<DataUnit>("unitname1");

    }

    bool FunctionTest::tests(Logger* logger)
    {

        auto result = true;
        result = FunctionTest::test_create_objects(logger) && result;
        result = FunctionTest::test_for(logger) && result;
        result = FunctionTest::test_if_(logger) && result;
        result = FunctionTest::std_functions(logger) && result;
        result = FunctionTest::test_1(logger) && result;
        result = FunctionTest::test_while(logger) && result;
        return result;

    }

    bool FunctionTest::test_create_objects(Logger* logger)
    {

        auto pointer = make_intrusive<FunctionTest>();
        pointer->initialize();
        auto result = pointer->initialized;
        result = result && pointer->data_unit != nullptr;
        return result;

    }

    bool FunctionTest::test_for(Logger* logger)
    {

        auto result = true;
        auto units = DataStorage::shared().get_units_keys();
        int index = 0;
        int count = list_size(units);
        for(auto& pair : units)
        {
            ++index;
        }
        result = result && index == count;

        index = 0;
        for(int i=0; i<count; ++i)
        {
            ++index;
        }
        result = result && index == count;

        index = 0;
        for(int i=count; i>0; --i)
        {
            ++index;
        }
        result = result && index == count;

        index = 0;
        auto count2 = count*2;
        for(int i=0; i<count2; i+=2)
        {
            ++index;
        }
        result = result && index == count;

        index = 0;
        int step = 2;
        for(int i=0; i<10; i+=step)
        {
            index += 1;
        }
        result = result && index == 5;

        TestToFor test_value;
        test_value.some_count = 2;
        index = 0;
        for(int i=0; i<test_value.some_count; ++i)
        {
            index += 1;
        }
        result = result && index == test_value.some_count;

        return result;

    }

    bool FunctionTest::test_while(Logger* logger)
    {

        auto result = true;
        std::vector<int> test_vector;

        int index = 0;
        while(index < 10)
        {
            list_push(test_vector, index);
            index++;
        }
        result = list_size(test_vector) == 10;
        return result;

    }

    bool FunctionTest::test_if_(Logger* logger)
    {

        bool result = true;
        if(true)
        {
            result = result && true;
        }
        else
        {
            result = false;
        }
        if(true)
        {
            result = result && true;
            result = result && true;
        }
        else
        {
            result = false;
            result = false;
        }
        if(true)
        result = result && true;
        else
        result = false;

        if(true)
        result = result && true;
        else if(false)
        result = false;
        return result;

    }

    bool FunctionTest::std_functions(Logger* logger)
    {

        auto result = true;
        auto pointer = make_intrusive<FunctionTest>();

        list_push(pointer->list, 1);
        result = result && list_size(pointer->list) == 1;

        pointer->map[1] = 1;
        result = result && map_size(pointer->map) == 1;
        result = result && in_map(1, pointer->map);

        return result;

    }

    bool FunctionTest::test_1(Logger* logger)
    {

        auto user = make_intrusive<TestUser1>();
        auto result = FunctionTest::test_1_1(user);
        return result;

    }

    bool FunctionTest::test_1_1(TestUser1* user)
    {

        auto result = user->result;
        return result;

    }

    void FunctionTest::retain()
    {
        ++this->_reference_counter;
    }

    int FunctionTest::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool FunctionTest::operator ==(const FunctionTest& rhs) const
    {
        bool result = true;
        result = result && this->initialized == rhs.initialized;
        result = result && ((this->data_unit == rhs.data_unit) || (this->data_unit != nullptr && rhs.data_unit != nullptr && *this->data_unit == *rhs.data_unit));
        result = result && this->list == rhs.list;
        result = result && this->map == rhs.map;
        return result;
    }

    bool FunctionTest::operator !=(const FunctionTest& rhs) const
    {

        return !(*this == rhs);
    }

    FunctionTest::FunctionTest(const FunctionTest& rhs)
    {

        this->operator=(rhs);
    }

    const FunctionTest& FunctionTest::operator =(const FunctionTest& rhs)
    {

        this->initialized = rhs.initialized;
        this->data_unit = rhs.data_unit;
        this->list = rhs.list;
        this->map = rhs.map;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string FunctionTest::get_type() const
    {
        return FunctionTest::TYPE;
    }

    void FunctionTest::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(data_unit, "data_unit");
        serializer.serialize(list, "list");
        serializer.serialize(map, "map");
    }

    void FunctionTest::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(data_unit, "data_unit");
        deserializer.deserialize(list, "list");
        deserializer.deserialize(map, "map");
    }

    void FunctionTest::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(data_unit, "data_unit");
        serializer.serialize(list, "list");
        serializer.serialize(map, "map");
    }

    void FunctionTest::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(data_unit, "data_unit");
        deserializer.deserialize(list, "list");
        deserializer.deserialize(map, "map");
    }

} // namespace mg
