#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataComplexMap.h"
#include "DataStorage.h"
#include "TestDataComplexMap.h"
#include "tests/ITestDataComplexMap.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestDataComplexMap::TYPE("TestDataComplexMap");

    
    TestDataComplexMap::TestDataComplexMap()
    {
    
    }

    TestDataComplexMap::~TestDataComplexMap(){}


void TestDataComplexMap::test_foo()
{

    
}

void TestDataComplexMap::test_deserialize()
{

        auto data = DataStorage::shared().get<DataComplexMap>("test");
        this->assertTrue(map_size(data->complex_map) == 1);
        this->assertTrue(list_size(data->complex_map.at(1)) == 2);
        this->assertTrue(data->complex_map.at(1)[0] == 1);
        this->assertTrue(data->complex_map.at(1)[1] == 2);
    
}

bool TestDataComplexMap::operator ==(const TestDataComplexMap& rhs) const
{

bool result = this->ITestDataComplexMap::operator ==(rhs);
return result;
}

bool TestDataComplexMap::operator !=(const TestDataComplexMap& rhs) const
{

return !(*this == rhs);
}

 TestDataComplexMap::TestDataComplexMap(const TestDataComplexMap& rhs)
{

this->operator=(rhs);
}

const TestDataComplexMap& TestDataComplexMap::operator =(const TestDataComplexMap& rhs)
{

this->ITestDataComplexMap::operator=(rhs);
return *this;
}

std::string TestDataComplexMap::get_type() const
{
return TestDataComplexMap::TYPE;
}

void TestDataComplexMap::serialize_xml(SerializerXml& serializer) const
{

}

void TestDataComplexMap::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestDataComplexMap::serialize_json(SerializerJson& serializer) const
{

}

void TestDataComplexMap::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
