#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Logger.h"
#include "TestCase.h"
#include <atomic>
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestCase::TYPE("TestCase");

    
    TestCase::TestCase()
    : logger(nullptr)
, result(true)
, _reference_counter(1)
{
    
    }

    TestCase::~TestCase(){}


void TestCase::initialize(Logger* logger)
{

        this->logger = logger;
    
}

void TestCase::add_result(bool result, const std::string& message)
{

        if(this->result && !result)
        {
            this->logger->message(this->get_type() + ":");
        }
        if(!result)
        {
            this->logger->message(" - Failed: " + message);
        }
        this->result = this->result && result;
    
}

void TestCase::assertTrue(bool expression, const std::string& message)
{

        this->add_result(expression, message);
    
}

void TestCase::assertFalse(bool expression, const std::string& message)
{

        this->add_result(!expression, message);
    
}

void TestCase::retain()
{
++this->_reference_counter;
}

int TestCase::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool TestCase::operator ==(const TestCase& rhs) const
{
bool result = true;
result = result && ((this->logger == rhs.logger) || (this->logger != nullptr && rhs.logger != nullptr && *this->logger == *rhs.logger));
result = result && this->result == rhs.result;
return result;
}

bool TestCase::operator !=(const TestCase& rhs) const
{

return !(*this == rhs);
}

 TestCase::TestCase(const TestCase& rhs)
{

this->operator=(rhs);
}

const TestCase& TestCase::operator =(const TestCase& rhs)
{

this->logger = rhs.logger;
this->result = rhs.result;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string TestCase::get_type() const
{
return TestCase::TYPE;
}

void TestCase::serialize_xml(SerializerXml& serializer) const
{

}

void TestCase::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestCase::serialize_json(SerializerJson& serializer) const
{

}

void TestCase::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
