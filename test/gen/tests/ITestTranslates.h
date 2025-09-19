#ifndef __mg_ITestTranslates_h__
#define __mg_ITestTranslates_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "TestCase.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class ITestTranslates : public TestCase
    {
    public:
        ITestTranslates();
        virtual ~ITestTranslates();
virtual void test_dummy_function() = 0;
void test_new_function();
virtual void test_map_remove_if() = 0;
virtual void test_map_do_if() = 0;
virtual void test_list_remove_if() = 0;
virtual void test_list_do_if() = 0;
virtual void test_list_do() = 0;
virtual void test_strcat() = 0;
virtual void test_strcat2() = 0;
virtual void test_create_objects() = 0;
virtual void test_list() = 0;
virtual void test_list_index() = 0;
virtual void test_map() = 0;
virtual void test_to_str() = 0;
virtual void test_str_to() = 0;
virtual void test_at() = 0;
virtual void test_list_remove() = 0;
virtual void test_list_clear() = 0;
virtual void test_list_resize() = 0;
virtual void test_string_size() = 0;
virtual void test_string_empty() = 0;
virtual void test_map_clear() = 0;
virtual void test_map_remove() = 0;
virtual void test_list_push() = 0;
virtual void test_list_insert() = 0;
virtual void test_list_erase() = 0;
virtual void test_list_truncate() = 0;
virtual void test_translate_declare_empty_string() = 0;
virtual void test_translate_for_with_size_t() = 0;
virtual void test_translate_dynamic_pointer_cast_intrusive() = 0;
virtual void test_min_max() = 0;
virtual void test_swap() = 0;
virtual void test_split() = 0;
void execute();
bool operator ==(const ITestTranslates& rhs) const;
bool operator !=(const ITestTranslates& rhs) const;
 ITestTranslates(const ITestTranslates& rhs);
const ITestTranslates& operator =(const ITestTranslates& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestTranslates_h__
