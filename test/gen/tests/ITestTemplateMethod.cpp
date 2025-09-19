#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestTemplateMethod.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestTemplateMethod::TYPE("ITestTemplateMethod");

    
    ITestTemplateMethod::ITestTemplateMethod()
    {
    
    }

    ITestTemplateMethod::~ITestTemplateMethod(){}


void ITestTemplateMethod::execute()
{
this->test_get();
}

bool ITestTemplateMethod::operator ==(const ITestTemplateMethod& rhs) const
{

bool result = true;
return result;
}

bool ITestTemplateMethod::operator !=(const ITestTemplateMethod& rhs) const
{

return !(*this == rhs);
}

 ITestTemplateMethod::ITestTemplateMethod(const ITestTemplateMethod& rhs)
{

this->operator=(rhs);
}

const ITestTemplateMethod& ITestTemplateMethod::operator =(const ITestTemplateMethod& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestTemplateMethod::get_type() const
{
return ITestTemplateMethod::TYPE;
}

void ITestTemplateMethod::serialize_xml(SerializerXml& serializer) const
{

}

void ITestTemplateMethod::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestTemplateMethod::serialize_json(SerializerJson& serializer) const
{

}

void ITestTemplateMethod::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
