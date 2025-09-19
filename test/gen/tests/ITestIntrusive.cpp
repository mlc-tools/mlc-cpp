#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestIntrusive.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestIntrusive::TYPE("ITestIntrusive");

    
    ITestIntrusive::ITestIntrusive()
    {
    
    }

    ITestIntrusive::~ITestIntrusive(){}


void ITestIntrusive::execute()
{
this->test_add_self_to_list();
this->test_remove_self_from_list();

}

bool ITestIntrusive::operator ==(const ITestIntrusive& rhs) const
{

bool result = true;
return result;
}

bool ITestIntrusive::operator !=(const ITestIntrusive& rhs) const
{

return !(*this == rhs);
}

 ITestIntrusive::ITestIntrusive(const ITestIntrusive& rhs)
{

this->operator=(rhs);
}

const ITestIntrusive& ITestIntrusive::operator =(const ITestIntrusive& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestIntrusive::get_type() const
{
return ITestIntrusive::TYPE;
}

void ITestIntrusive::serialize_xml(SerializerXml& serializer) const
{

}

void ITestIntrusive::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestIntrusive::serialize_json(SerializerJson& serializer) const
{

}

void ITestIntrusive::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
