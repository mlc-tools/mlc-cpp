#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "AllTests.h"
#include "AllTypes.h"
#include "ExceptionTest.h"
#include "FunctionTest.h"
#include "TestFactory.h"
#include "data/DataUnit.h"
#include "tests/Logger.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string AllTests::TYPE("AllTests");

    AllTests::AllTests()
    : _reference_counter(1)
    {

    }

    AllTests::~AllTests(){}

    bool AllTests::run(Logger* logger)
    {

        bool result = true;
        result = FunctionTest::tests(logger) && result;
        result = DataUnit::tests(logger) && result;
        result = AllTypes::tests(logger) && result;
        result = TestFactory::tests(logger) && result;
        result = ExceptionTest::test() && result;

        return result;

    }

    void AllTests::retain()
    {
        ++this->_reference_counter;
    }

    int AllTests::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool AllTests::operator ==(const AllTests& rhs) const
    {
        bool result = true;
        return result;
    }

    bool AllTests::operator !=(const AllTests& rhs) const
    {

        return !(*this == rhs);
    }

    AllTests::AllTests(const AllTests& rhs)
    {

        this->operator=(rhs);
    }

    const AllTests& AllTests::operator =(const AllTests& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string AllTests::get_type() const
    {
        return AllTests::TYPE;
    }

    void AllTests::serialize_xml(SerializerXml& serializer) const
    {

    }

    void AllTests::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void AllTests::serialize_json(SerializerJson& serializer) const
    {

    }

    void AllTests::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
