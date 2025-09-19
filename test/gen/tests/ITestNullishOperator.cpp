#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestNullishOperator.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestNullishOperator::TYPE("ITestNullishOperator");

    
    ITestNullishOperator::ITestNullishOperator()
    {
    
    }

    ITestNullishOperator::~ITestNullishOperator(){}


void ITestNullishOperator::execute()
{
this->test_dummy_function();
}

bool ITestNullishOperator::operator ==(const ITestNullishOperator& rhs) const
{

bool result = true;
return result;
}

bool ITestNullishOperator::operator !=(const ITestNullishOperator& rhs) const
{

return !(*this == rhs);
}

 ITestNullishOperator::ITestNullishOperator(const ITestNullishOperator& rhs)
{

this->operator=(rhs);
}

const ITestNullishOperator& ITestNullishOperator::operator =(const ITestNullishOperator& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestNullishOperator::get_type() const
{
return ITestNullishOperator::TYPE;
}

void ITestNullishOperator::serialize_xml(SerializerXml& serializer) const
{

}

void ITestNullishOperator::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestNullishOperator::serialize_json(SerializerJson& serializer) const
{

}

void ITestNullishOperator::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
