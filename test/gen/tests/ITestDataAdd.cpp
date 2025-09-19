#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestDataAdd.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestDataAdd::TYPE("ITestDataAdd");

    
    ITestDataAdd::ITestDataAdd()
    {
    
    }

    ITestDataAdd::~ITestDataAdd(){}


void ITestDataAdd::execute()
{
this->test_test();

}

bool ITestDataAdd::operator ==(const ITestDataAdd& rhs) const
{

bool result = true;
return result;
}

bool ITestDataAdd::operator !=(const ITestDataAdd& rhs) const
{

return !(*this == rhs);
}

 ITestDataAdd::ITestDataAdd(const ITestDataAdd& rhs)
{

this->operator=(rhs);
}

const ITestDataAdd& ITestDataAdd::operator =(const ITestDataAdd& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestDataAdd::get_type() const
{
return ITestDataAdd::TYPE;
}

void ITestDataAdd::serialize_xml(SerializerXml& serializer) const
{

}

void ITestDataAdd::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestDataAdd::serialize_json(SerializerJson& serializer) const
{

}

void ITestDataAdd::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
