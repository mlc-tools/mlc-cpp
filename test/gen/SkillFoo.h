#ifndef __mg_SkillFoo_h__
#define __mg_SkillFoo_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "SkillBase.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class SkillFoo : public SkillBase
    {
    public:
        SkillFoo();
        ~SkillFoo();
bool operator ==(const SkillFoo& rhs) const;
bool operator !=(const SkillFoo& rhs) const;
 SkillFoo(const SkillFoo& rhs);
const SkillFoo& operator =(const SkillFoo& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

int foo;
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_SkillFoo_h__
