#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataListListBool.h"
#include "DataStorage.h"
#include "TestDataListListBool.h"
#include "tests/ITestDataListListBool.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestDataListListBool::TYPE("TestDataListListBool");

    
    TestDataListListBool::TestDataListListBool()
    {
    
    }

    TestDataListListBool::~TestDataListListBool(){}


void TestDataListListBool::test_foo()
{

        auto data = DataStorage::shared().get<DataListListBool>("test");
        this->assertTrue(list_size(data->list_list_bool) == 2);
        this->assertTrue(list_size(data->list_list_bool[0]) == 2);
        this->assertTrue(list_size(data->list_list_bool[1]) == 2);
        this->assertTrue(data->list_list_bool[0][0] == true);
        this->assertTrue(data->list_list_bool[0][1] == false);
        this->assertTrue(data->list_list_bool[1][0] == false);
        this->assertTrue(data->list_list_bool[1][1] == true);
    
}

bool TestDataListListBool::operator ==(const TestDataListListBool& rhs) const
{

bool result = this->ITestDataListListBool::operator ==(rhs);
return result;
}

bool TestDataListListBool::operator !=(const TestDataListListBool& rhs) const
{

return !(*this == rhs);
}

 TestDataListListBool::TestDataListListBool(const TestDataListListBool& rhs)
{

this->operator=(rhs);
}

const TestDataListListBool& TestDataListListBool::operator =(const TestDataListListBool& rhs)
{

this->ITestDataListListBool::operator=(rhs);
return *this;
}

std::string TestDataListListBool::get_type() const
{
return TestDataListListBool::TYPE;
}

void TestDataListListBool::serialize_xml(SerializerXml& serializer) const
{

}

void TestDataListListBool::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestDataListListBool::serialize_json(SerializerJson& serializer) const
{

}

void TestDataListListBool::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
