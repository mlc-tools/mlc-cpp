#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataAdd.h"
#include "DataStorage.h"
#include "TestDataAdd.h"
#include "data/DataUnit.h"
#include "tests/ITestDataAdd.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestDataAdd::TYPE("TestDataAdd");

    
    TestDataAdd::TestDataAdd()
    {
    
    }

    TestDataAdd::~TestDataAdd(){}


void TestDataAdd::test_test()
{

        DataStorage::shared().get<DataAdd>("add_data");
        auto keys = DataStorage::shared().get_adds_keys();
        this->assertTrue(in_list("add_data", keys), "Additional data in DataStorage");

        auto unit = DataStorage::shared().get<DataUnit>("unitname1");
        this->assertTrue(unit->empty == nullptr, "empty is null");
    
}

bool TestDataAdd::operator ==(const TestDataAdd& rhs) const
{

bool result = this->ITestDataAdd::operator ==(rhs);
return result;
}

bool TestDataAdd::operator !=(const TestDataAdd& rhs) const
{

return !(*this == rhs);
}

 TestDataAdd::TestDataAdd(const TestDataAdd& rhs)
{

this->operator=(rhs);
}

const TestDataAdd& TestDataAdd::operator =(const TestDataAdd& rhs)
{

this->ITestDataAdd::operator=(rhs);
return *this;
}

std::string TestDataAdd::get_type() const
{
return TestDataAdd::TYPE;
}

void TestDataAdd::serialize_xml(SerializerXml& serializer) const
{

}

void TestDataAdd::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestDataAdd::serialize_json(SerializerJson& serializer) const
{

}

void TestDataAdd::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
