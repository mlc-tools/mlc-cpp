#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BarClassFromBase.h"
#include "BaseClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string BarClassFromBase::TYPE("BarClassFromBase");

    BarClassFromBase::BarClassFromBase()
    {

    }

    BarClassFromBase::~BarClassFromBase(){}

    bool BarClassFromBase::operator ==(const BarClassFromBase& rhs) const
    {

        bool result = this->BaseClass::operator ==(rhs);
        return result;
    }

    bool BarClassFromBase::operator !=(const BarClassFromBase& rhs) const
    {

        return !(*this == rhs);
    }

    BarClassFromBase::BarClassFromBase(const BarClassFromBase& rhs)
    {

        this->operator=(rhs);
    }

    const BarClassFromBase& BarClassFromBase::operator =(const BarClassFromBase& rhs)
    {

        this->BaseClass::operator=(rhs);
        return *this;
    }

    std::string BarClassFromBase::get_type() const
    {
        return BarClassFromBase::TYPE;
    }

    void BarClassFromBase::serialize_xml(SerializerXml& serializer) const
    {

    }

    void BarClassFromBase::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void BarClassFromBase::serialize_json(SerializerJson& serializer) const
    {

    }

    void BarClassFromBase::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
