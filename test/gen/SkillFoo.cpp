#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "SkillBase.h"
#include "SkillFoo.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string SkillFoo::TYPE("SkillFoo");

    SkillFoo::SkillFoo()
    : foo(-1)
    {

    }

    SkillFoo::~SkillFoo(){}

    bool SkillFoo::operator ==(const SkillFoo& rhs) const
    {

        bool result = this->SkillBase::operator ==(rhs);
        result = result && this->foo == rhs.foo;
        return result;
    }

    bool SkillFoo::operator !=(const SkillFoo& rhs) const
    {

        return !(*this == rhs);
    }

    SkillFoo::SkillFoo(const SkillFoo& rhs)
    {

        this->operator=(rhs);
    }

    const SkillFoo& SkillFoo::operator =(const SkillFoo& rhs)
    {

        this->SkillBase::operator=(rhs);
        this->foo = rhs.foo;
        return *this;
    }

    std::string SkillFoo::get_type() const
    {
        return SkillFoo::TYPE;
    }

    void SkillFoo::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(foo, "foo", int(-1));
    }

    void SkillFoo::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(foo, "foo", int(-1));
    }

    void SkillFoo::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(foo, "foo", int(-1));
    }

    void SkillFoo::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(foo, "foo", int(-1));
    }

} // namespace mg
