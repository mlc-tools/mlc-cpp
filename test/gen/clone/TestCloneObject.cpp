#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../data/DataUnit.h"
#include "../tests/ITestCloneObject.h"
#include "CloneObject.h"
#include "CloneObject2.h"
#include "TestCloneObject.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestCloneObject::TYPE("TestCloneObject");

    
    TestCloneObject::TestCloneObject()
    {
    
    }

    TestCloneObject::~TestCloneObject(){}


void TestCloneObject::test_foo()
{

        auto object_a = make_intrusive<CloneObject>();
        object_a->t = 1;

        auto object_b = clone_object<CloneObject>(object_a);
        this->assertEqual(object_a->t, object_b->t);
    
}

void TestCloneObject::test_clone2()
{

        auto object_a = make_intrusive<CloneObject2>();
        object_a->t = 1;
        object_a->ptr = make_intrusive<CloneObject>();
        object_a->ptr->t = 10;

        auto object_b = clone_object<CloneObject2>(object_a);
        this->assertEqual(object_a->t, object_b->t);
        this->assertEqual(object_a->ptr->t, object_b->ptr->t);
    
}

void TestCloneObject::test_copy_list_with_null()
{

        auto obj1 = make_intrusive<CloneObject2>();
        list_push(obj1->list1, make_intrusive<CloneObject2>());
        list_push(obj1->list1, nullptr);
        list_push(obj1->list1, make_intrusive<CloneObject2>());
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertEqual(list_size(obj2->list1), 3);
    
}

void TestCloneObject::test_copy_list_with_null2()
{

        auto obj1 = make_intrusive<CloneObject2>();
        list_push(obj1->list2, DataStorage::shared().get<DataUnit>("unitname1"));
        list_push(obj1->list2, nullptr);
        list_push(obj1->list2, DataStorage::shared().get<DataUnit>("unitname1"));
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertEqual(list_size(obj2->list2), 3);
    
}

void TestCloneObject::test_copy_map1()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map1[nullptr] = nullptr;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(in_map(nullptr, obj2->map1));
        this->assertTrue(obj2->map1.at(nullptr) == nullptr);
    
}

void TestCloneObject::test_copy_map2()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map2[nullptr] = nullptr;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(in_map(nullptr, obj2->map2));
        
    
}

void TestCloneObject::test_copy_map3()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map3[nullptr] = nullptr;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(in_map(nullptr, obj2->map3));
        
    
}

void TestCloneObject::test_copy_map4()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map4[nullptr] = nullptr;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(map_size(obj2->map4) == 1);
        
        
    
}

void TestCloneObject::test_copy_map5()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map5[nullptr] = 1;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(map_size(obj2->map5) == 1);
        this->assertTrue(obj2->map5.at(nullptr) == 1);
    
}

void TestCloneObject::test_copy_map6()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map6[1] = nullptr;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(map_size(obj2->map6) == 1);
        this->assertTrue(obj2->map6.at(1) == nullptr);
    
}

void TestCloneObject::test_copy_map7()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map7[nullptr] = 1;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(map_size(obj2->map7) == 1);
        this->assertTrue(obj2->map7.at(nullptr) == 1);
    
}

void TestCloneObject::test_copy_map8()
{

        auto obj1 = make_intrusive<CloneObject2>();
        obj1->map8[1] = nullptr;
        auto obj2 = clone_object<CloneObject2>(obj1);
        this->assertTrue(map_size(obj2->map8) == 1);
    
}

bool TestCloneObject::operator ==(const TestCloneObject& rhs) const
{

bool result = this->ITestCloneObject::operator ==(rhs);
return result;
}

bool TestCloneObject::operator !=(const TestCloneObject& rhs) const
{

return !(*this == rhs);
}

 TestCloneObject::TestCloneObject(const TestCloneObject& rhs)
{

this->operator=(rhs);
}

const TestCloneObject& TestCloneObject::operator =(const TestCloneObject& rhs)
{

this->ITestCloneObject::operator=(rhs);
return *this;
}

std::string TestCloneObject::get_type() const
{
return TestCloneObject::TYPE;
}

void TestCloneObject::serialize_xml(SerializerXml& serializer) const
{

}

void TestCloneObject::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestCloneObject::serialize_json(SerializerJson& serializer) const
{

}

void TestCloneObject::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
