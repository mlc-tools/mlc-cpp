#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestDataComplexMap.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestDataComplexMap::TYPE("ITestDataComplexMap");

    
    ITestDataComplexMap::ITestDataComplexMap()
    {
    
    }

    ITestDataComplexMap::~ITestDataComplexMap(){}


void ITestDataComplexMap::execute()
{
this->test_foo();
}

bool ITestDataComplexMap::operator ==(const ITestDataComplexMap& rhs) const
{

bool result = true;
return result;
}

bool ITestDataComplexMap::operator !=(const ITestDataComplexMap& rhs) const
{

return !(*this == rhs);
}

 ITestDataComplexMap::ITestDataComplexMap(const ITestDataComplexMap& rhs)
{

this->operator=(rhs);
}

const ITestDataComplexMap& ITestDataComplexMap::operator =(const ITestDataComplexMap& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestDataComplexMap::get_type() const
{
return ITestDataComplexMap::TYPE;
}

void ITestDataComplexMap::serialize_xml(SerializerXml& serializer) const
{

}

void ITestDataComplexMap::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestDataComplexMap::serialize_json(SerializerJson& serializer) const
{

}

void ITestDataComplexMap::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
