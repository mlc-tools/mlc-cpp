#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../core/CommandBase.h"
#include "TestIncludesGet.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string TestIncludesGet::TYPE("TestIncludesGet");

    
    TestIncludesGet::TestIncludesGet()
    : _reference_counter(1)
{
    
    }

    TestIncludesGet::~TestIncludesGet(){}


intrusive_ptr<CommandBase> TestIncludesGet::get()
{

        return make_intrusive<CommandBase>();
    
}

void TestIncludesGet::retain()
{
++this->_reference_counter;
}

int TestIncludesGet::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool TestIncludesGet::operator ==(const TestIncludesGet& rhs) const
{
bool result = true;
return result;
}

bool TestIncludesGet::operator !=(const TestIncludesGet& rhs) const
{

return !(*this == rhs);
}

 TestIncludesGet::TestIncludesGet(const TestIncludesGet& rhs)
{

this->operator=(rhs);
}

const TestIncludesGet& TestIncludesGet::operator =(const TestIncludesGet& rhs)
{

this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string TestIncludesGet::get_type() const
{
return TestIncludesGet::TYPE;
}

void TestIncludesGet::serialize_xml(SerializerXml& serializer) const
{

}

void TestIncludesGet::deserialize_xml(DeserializerXml& deserializer)
{

}

void TestIncludesGet::serialize_json(SerializerJson& serializer) const
{

}

void TestIncludesGet::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
