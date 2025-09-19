#ifndef __mg_TestTranslates_h__
#define __mg_TestTranslates_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "TestTranslatesModel.h"
#include "tests/ITestTranslates.h"
#include <map>
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TestTranslates : public ITestTranslates
    {
    public:
        TestTranslates();
        ~TestTranslates();
void test_map_remove_if();
void test_map_do_if();
void test_list_remove_if();
void test_list_do_if();
void test_list_do();
void test_dummy_function();
void test_strcat();
void test_strcat2();
void test_create_objects();
void test_list();
void test_list_index();
void test_map();
void test_to_str();
void test_str_to();
void test_at();
void test_list_remove();
void test_list_clear();
void test_list_resize();
void test_string_size();
void test_string_empty();
void test_map_clear();
void test_map_remove();
void test_list_push();
void test_list_insert();
void test_list_erase();
void test_list_truncate();
void test_translate_declare_empty_string();
void test_translate_for_with_size_t();
void test_translate_dynamic_pointer_cast_intrusive();
void test_min_max();
void test_swap();
void test_split();
bool operator ==(const TestTranslates& rhs) const;
bool operator !=(const TestTranslates& rhs) const;
 TestTranslates(const TestTranslates& rhs);
const TestTranslates& operator =(const TestTranslates& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

std::map<std::string, intrusive_ptr<TestTranslatesModel>> test_map_models;
std::vector<int> test_list_lambda;
std::vector<intrusive_ptr<TestTranslatesModel>> test_list_lambda2;
std::vector<int> test_list_swap1;
std::vector<int> test_list_swap2;
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestTranslates_h__
