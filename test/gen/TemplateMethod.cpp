#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BaseClass.h"
#include "TemplateMethod.h"
#include <map>
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TemplateMethod::TYPE("TemplateMethod");

    TemplateMethod::TemplateMethod()
    : members()
    , _reference_counter(1)
    {

    }

    TemplateMethod::~TemplateMethod(){}

    void TemplateMethod::retain()
    {
        ++this->_reference_counter;
    }

    int TemplateMethod::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool TemplateMethod::operator ==(const TemplateMethod& rhs) const
    {
        bool result = true;
        result = result && this->members == rhs.members;
        return result;
    }

    bool TemplateMethod::operator !=(const TemplateMethod& rhs) const
    {

        return !(*this == rhs);
    }

    TemplateMethod::TemplateMethod(const TemplateMethod& rhs)
    {

        this->operator=(rhs);
    }

    const TemplateMethod& TemplateMethod::operator =(const TemplateMethod& rhs)
    {

        this->members = rhs.members;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string TemplateMethod::get_type() const
    {
        return TemplateMethod::TYPE;
    }

    void TemplateMethod::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(members, "members");
    }

    void TemplateMethod::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(members, "members");
    }

    void TemplateMethod::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(members, "members");
    }

    void TemplateMethod::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(members, "members");
    }

} // namespace mg
