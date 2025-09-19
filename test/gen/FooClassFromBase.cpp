#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BaseClass.h"
#include "FooClassFromBase.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string FooClassFromBase::TYPE("FooClassFromBase");

    FooClassFromBase::FooClassFromBase()
    {

    }

    FooClassFromBase::~FooClassFromBase(){}

    bool FooClassFromBase::operator ==(const FooClassFromBase& rhs) const
    {

        bool result = this->BaseClass::operator ==(rhs);
        return result;
    }

    bool FooClassFromBase::operator !=(const FooClassFromBase& rhs) const
    {

        return !(*this == rhs);
    }

    FooClassFromBase::FooClassFromBase(const FooClassFromBase& rhs)
    {

        this->operator=(rhs);
    }

    const FooClassFromBase& FooClassFromBase::operator =(const FooClassFromBase& rhs)
    {

        this->BaseClass::operator=(rhs);
        return *this;
    }

    std::string FooClassFromBase::get_type() const
    {
        return FooClassFromBase::TYPE;
    }

    void FooClassFromBase::serialize_xml(SerializerXml& serializer) const
    {

    }

    void FooClassFromBase::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void FooClassFromBase::serialize_json(SerializerJson& serializer) const
    {

    }

    void FooClassFromBase::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
