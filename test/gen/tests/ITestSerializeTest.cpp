#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestSerializeTest.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestSerializeTest::TYPE("ITestSerializeTest");

    
    ITestSerializeTest::ITestSerializeTest()
    {
    
    }

    ITestSerializeTest::~ITestSerializeTest(){}


void ITestSerializeTest::execute()
{
this->test_serialize_python();

}

bool ITestSerializeTest::operator ==(const ITestSerializeTest& rhs) const
{

bool result = true;
return result;
}

bool ITestSerializeTest::operator !=(const ITestSerializeTest& rhs) const
{

return !(*this == rhs);
}

 ITestSerializeTest::ITestSerializeTest(const ITestSerializeTest& rhs)
{

this->operator=(rhs);
}

const ITestSerializeTest& ITestSerializeTest::operator =(const ITestSerializeTest& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestSerializeTest::get_type() const
{
return ITestSerializeTest::TYPE;
}

void ITestSerializeTest::serialize_xml(SerializerXml& serializer) const
{

}

void ITestSerializeTest::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestSerializeTest::serialize_json(SerializerJson& serializer) const
{

}

void ITestSerializeTest::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
