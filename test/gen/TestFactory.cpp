#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "Request.h"
#include "RequestBar.h"
#include "TestFactory.h"
#include "tests/Logger.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestFactory::TYPE("TestFactory");

    TestFactory::TestFactory()
    : _reference_counter(1)
    {

    }

    TestFactory::~TestFactory(){}

    bool TestFactory::tests(Logger* logger)
    {

        bool result = true;
        result = TestFactory::test(logger) && result;
        return result;

    }

    bool TestFactory::test(Logger* logger)
    {

        bool result = true;

        auto command = make_intrusive<RequestBar>();
        #if SERIALIZE_FORMAT == JSON
        auto string = serialize_command_to_json<RequestBar>(command);
        auto command2 = create_command_from_json<Request>(string);
        #endif
        #if SERIALIZE_FORMAT == XML
        auto string = serialize_command_to_xml<RequestBar>(command);
        auto command2 = create_command_from_xml<Request>(string);
        #endif
        result = (command2 != nullptr) && result;
        if(result)
        {
            result = (command2->get_type() == command->get_type()) && result;
        }
        return result;

    }

    void TestFactory::retain()
    {
        ++this->_reference_counter;
    }

    int TestFactory::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool TestFactory::operator ==(const TestFactory& rhs) const
    {
        bool result = true;
        return result;
    }

    bool TestFactory::operator !=(const TestFactory& rhs) const
    {

        return !(*this == rhs);
    }

    TestFactory::TestFactory(const TestFactory& rhs)
    {

        this->operator=(rhs);
    }

    const TestFactory& TestFactory::operator =(const TestFactory& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string TestFactory::get_type() const
    {
        return TestFactory::TYPE;
    }

    void TestFactory::serialize_xml(SerializerXml& serializer) const
    {

    }

    void TestFactory::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void TestFactory::serialize_json(SerializerJson& serializer) const
    {

    }

    void TestFactory::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
