#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestObservableClass.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestObservableClass::TYPE("ITestObservableClass");

    
    ITestObservableClass::ITestObservableClass()
    {
    
    }

    ITestObservableClass::~ITestObservableClass(){}


void ITestObservableClass::execute()
{

}

bool ITestObservableClass::operator ==(const ITestObservableClass& rhs) const
{

bool result = true;
return result;
}

bool ITestObservableClass::operator !=(const ITestObservableClass& rhs) const
{

return !(*this == rhs);
}

 ITestObservableClass::ITestObservableClass(const ITestObservableClass& rhs)
{

this->operator=(rhs);
}

const ITestObservableClass& ITestObservableClass::operator =(const ITestObservableClass& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestObservableClass::get_type() const
{
return ITestObservableClass::TYPE;
}

void ITestObservableClass::serialize_xml(SerializerXml& serializer) const
{

}

void ITestObservableClass::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestObservableClass::serialize_json(SerializerJson& serializer) const
{

}

void ITestObservableClass::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
