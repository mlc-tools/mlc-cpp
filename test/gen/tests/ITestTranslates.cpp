#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestTranslates.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestTranslates::TYPE("ITestTranslates");

    
    ITestTranslates::ITestTranslates()
    {
    
    }

    ITestTranslates::~ITestTranslates(){}


void ITestTranslates::test_new_function()
{

}

void ITestTranslates::execute()
{
this->test_dummy_function();this->test_new_function();
}

bool ITestTranslates::operator ==(const ITestTranslates& rhs) const
{

bool result = true;
return result;
}

bool ITestTranslates::operator !=(const ITestTranslates& rhs) const
{

return !(*this == rhs);
}

 ITestTranslates::ITestTranslates(const ITestTranslates& rhs)
{

this->operator=(rhs);
}

const ITestTranslates& ITestTranslates::operator =(const ITestTranslates& rhs)
{

this->TestCase::operator=(rhs);
return *this;
}

std::string ITestTranslates::get_type() const
{
return ITestTranslates::TYPE;
}

void ITestTranslates::serialize_xml(SerializerXml& serializer) const
{

}

void ITestTranslates::deserialize_xml(DeserializerXml& deserializer)
{

}

void ITestTranslates::serialize_json(SerializerJson& serializer) const
{

}

void ITestTranslates::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
