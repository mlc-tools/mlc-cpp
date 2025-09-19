#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "TestIncludes.h"
#include "TestIncludesGet.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestIncludes::TYPE("TestIncludes");

    TestIncludes::TestIncludes()
    : _reference_counter(1)
    {

    }

    TestIncludes::~TestIncludes(){}

    void TestIncludes::get()
    {

        auto get = make_intrusive<TestIncludesGet>();
        auto command = get->get();
        command->user_id = 0;

    }

    std::vector<int> TestIncludes::get_vector()
    {

        return std::vector<int>();

    }

    void TestIncludes::retain()
    {
        ++this->_reference_counter;
    }

    int TestIncludes::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool TestIncludes::operator ==(const TestIncludes& rhs) const
    {
        bool result = true;
        return result;
    }

    bool TestIncludes::operator !=(const TestIncludes& rhs) const
    {

        return !(*this == rhs);
    }

    TestIncludes::TestIncludes(const TestIncludes& rhs)
    {

        this->operator=(rhs);
    }

    const TestIncludes& TestIncludes::operator =(const TestIncludes& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string TestIncludes::get_type() const
    {
        return TestIncludes::TYPE;
    }

    void TestIncludes::serialize_xml(SerializerXml& serializer) const
    {

    }

    void TestIncludes::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void TestIncludes::serialize_json(SerializerJson& serializer) const
    {

    }

    void TestIncludes::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
