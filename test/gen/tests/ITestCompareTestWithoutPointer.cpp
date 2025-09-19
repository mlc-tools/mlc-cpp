#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestCompareTestWithoutPointer.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestCompareTestWithoutPointer::TYPE("ITestCompareTestWithoutPointer");

    
    ITestCompareTestWithoutPointer::ITestCompareTestWithoutPointer()
    {
    
    }

    ITestCompareTestWithoutPointer::~ITestCompareTestWithoutPointer(){}


void ITestCompareTestWithoutPointer::execute()
{
this->test_test();
}

bool ITestCompareTestWithoutPointer::operator ==(const ITestCompareTestWithoutPointer& rhs) const
{

bool result = true;
return result;
}

bool ITestCompareTestWithoutPointer::operator !=(const ITestCompareTestWithoutPointer& rhs) const
{

return !(*this == rhs);
}

 ITestCompareTestWithoutPointer::ITestCompareTestWithoutPointer(const ITestCompareTestWithoutPointer& rhs)
{

this->operator=(rhs);
}

const ITestCompareTestWithoutPointer& ITestCompareTestWithoutPointer::operator =(const ITestCompareTestWithoutPointer& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestCompareTestWithoutPointer::get_type() const
{
return ITestCompareTestWithoutPointer::TYPE;
}

void ITestCompareTestWithoutPointer::serialize_xml(SerializerXml& serializer) const
{

}

void ITestCompareTestWithoutPointer::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestCompareTestWithoutPointer::serialize_json(SerializerJson& serializer) const
{

}

void ITestCompareTestWithoutPointer::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
