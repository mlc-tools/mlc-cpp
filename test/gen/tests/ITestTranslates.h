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
