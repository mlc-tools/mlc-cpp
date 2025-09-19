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
this->test_dummy_function();
this->test_new_function();
this->test_map_remove_if();
this->test_map_do_if();
this->test_list_remove_if();
this->test_list_do_if();
this->test_list_do();
this->test_strcat();
this->test_strcat2();
this->test_create_objects();
this->test_list();
this->test_list_index();
this->test_map();
this->test_to_str();
this->test_str_to();
this->test_at();
this->test_list_remove();
this->test_list_clear();
this->test_list_resize();
this->test_string_size();
this->test_string_empty();
this->test_map_clear();
this->test_map_remove();
this->test_list_push();
this->test_list_insert();
this->test_list_erase();
this->test_list_truncate();
this->test_translate_declare_empty_string();
this->test_translate_for_with_size_t();
this->test_translate_dynamic_pointer_cast_intrusive();
this->test_min_max();
this->test_swap();
this->test_split();

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
