#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "Intrusive.h"
#include "TestIntrusive.h"
#include "tests/ITestIntrusive.h"
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestIntrusive::TYPE("TestIntrusive");

    TestIntrusive::TestIntrusive()
    : pointers()
    {

    }

    TestIntrusive::~TestIntrusive(){}

    void TestIntrusive::test_add_self_to_list()
    {

        auto a = make_intrusive<Intrusive>();
        auto b = make_intrusive<Intrusive>();
        auto c = make_intrusive<Intrusive>();

        list_push(this->pointers, a);
        list_push(this->pointers, nullptr);
        b->add_self_to_list(this->pointers);
        c->add_self_to_list(this->pointers);
        this->assertEqual(list_size(this->pointers), 4, "list size == 4");

        c->remove_self_from_list(this->pointers);
        list_remove(this->pointers, a);
        list_remove(this->pointers, nullptr);

        this->assertEqual(list_size(this->pointers), 1, "list size == 1");

    }

    void TestIntrusive::test_remove_self_from_list()
    {

    }

    bool TestIntrusive::operator ==(const TestIntrusive& rhs) const
    {

        bool result = this->ITestIntrusive::operator ==(rhs);
        result = result && this->pointers == rhs.pointers;
        return result;
    }

    bool TestIntrusive::operator !=(const TestIntrusive& rhs) const
    {

        return !(*this == rhs);
    }

    TestIntrusive::TestIntrusive(const TestIntrusive& rhs)
    {

        this->operator=(rhs);
    }

    const TestIntrusive& TestIntrusive::operator =(const TestIntrusive& rhs)
    {

        this->ITestIntrusive::operator=(rhs);
        this->pointers = rhs.pointers;
        return *this;
    }

    std::string TestIntrusive::get_type() const
    {
        return TestIntrusive::TYPE;
    }

    void TestIntrusive::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(pointers, "pointers");
    }

    void TestIntrusive::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(pointers, "pointers");
    }

    void TestIntrusive::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(pointers, "pointers");
    }

    void TestIntrusive::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(pointers, "pointers");
    }

} // namespace mg
