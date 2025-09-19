#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../Resource.h"
#include "../SkillBase.h"
#include "../SkillFoo.h"
#include "../tests/ITestDataDict.h"
#include "DataDict.h"
#include "DataUnit.h"
#include "TestDataDict.h"
#include <map>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestDataDict::TYPE("TestDataDict");

    
    TestDataDict::TestDataDict()
    : resources()
{
    
    }

    TestDataDict::~TestDataDict(){}


void TestDataDict::test_resources_in_map()
{

        auto data = DataStorage::shared().get<DataDict>("default");
        this->assertTrue(in_map(Resource::gold, data->resources));
        this->assertTrue(in_map(Resource::gems, data->resources));
        this->assertTrue(data->resources.at(Resource::gold) == 10);
        this->assertTrue(data->resources.at(Resource::gems) == 20);

        map_clear(this->resources);
        for (auto&& pair : data->resources)
{
    auto& res = pair.first;
    auto& count = pair.second;
    (void)res; // don't generate 'Unused variable' warning
    (void)count; // don't generate 'Unused variable' warning
            this->resources[res] = count;
        }

        this->assertTrue(in_map(Resource::gold, this->resources));
        this->assertTrue(in_map(Resource::gems, this->resources));
        this->assertTrue(this->resources.at(Resource::gold) == 10);
        this->assertTrue(this->resources.at(Resource::gems) == 20);
    
}

void TestDataDict::test_resources_def_value()
{

        auto data = DataStorage::shared().get<DataDict>("default");
        this->assertTrue(data->string_with_def_value == "default");
    
}

void TestDataDict::test_skills()
{

        auto unit1 = DataStorage::shared().get<DataUnit>("unitname1");
        auto unit2 = DataStorage::shared().get<DataUnit>("unitname2");
        this->assertTrue(unit1->skill->get_type() == SkillFoo::TYPE);
        auto skill = dynamic_pointer_cast_intrusive<SkillFoo>(unit1->skill);
        this->assertTrue(skill->foo == 1);
        this->assertTrue(unit2->skill->get_type() == SkillBase::TYPE);

    
}

bool TestDataDict::operator ==(const TestDataDict& rhs) const
{

bool result = this->ITestDataDict::operator ==(rhs);
result = result && this->resources == rhs.resources;
return result;
}

bool TestDataDict::operator !=(const TestDataDict& rhs) const
{

return !(*this == rhs);
}

 TestDataDict::TestDataDict(const TestDataDict& rhs)
{

this->operator=(rhs);
}

const TestDataDict& TestDataDict::operator =(const TestDataDict& rhs)
{

this->ITestDataDict::operator=(rhs);
this->resources = rhs.resources;
return *this;
}

std::string TestDataDict::get_type() const
{
return TestDataDict::TYPE;
}

void TestDataDict::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(resources, "resources");
}

void TestDataDict::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(resources, "resources");
}

void TestDataDict::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(resources, "resources");
}

void TestDataDict::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(resources, "resources");
}

} // namespace mg
