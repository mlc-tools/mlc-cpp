#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestDataDict.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestDataDict::TYPE("ITestDataDict");

    
    ITestDataDict::ITestDataDict()
    {
    
    }

    ITestDataDict::~ITestDataDict(){}


void ITestDataDict::execute()
{

}

bool ITestDataDict::operator ==(const ITestDataDict& rhs) const
{

bool result = true;
return result;
}

bool ITestDataDict::operator !=(const ITestDataDict& rhs) const
{

return !(*this == rhs);
}

 ITestDataDict::ITestDataDict(const ITestDataDict& rhs)
{

this->operator=(rhs);
}

const ITestDataDict& ITestDataDict::operator =(const ITestDataDict& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestDataDict::get_type() const
{
return ITestDataDict::TYPE;
}

void ITestDataDict::serialize_xml(SerializerXml& serializer) const
{

}

void ITestDataDict::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestDataDict::serialize_json(SerializerJson& serializer) const
{

}

void ITestDataDict::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
