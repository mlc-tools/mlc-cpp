#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "IControllerDungeonBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string IControllerDungeonBase::TYPE("IControllerDungeonBase");

    IControllerDungeonBase::IControllerDungeonBase()
    {

    }

    IControllerDungeonBase::~IControllerDungeonBase()
    {
    }

    bool IControllerDungeonBase::operator ==(const IControllerDungeonBase& rhs) const
    {
        bool result = true;
        return result;
    }

    bool IControllerDungeonBase::operator !=(const IControllerDungeonBase& rhs) const
    {
        return !(*this == rhs);
    }

    IControllerDungeonBase::IControllerDungeonBase(const IControllerDungeonBase& rhs)
    {
        this->operator=(rhs);
    }

    const IControllerDungeonBase& IControllerDungeonBase::operator =(const IControllerDungeonBase& rhs)
    {
        return *this;
    }

    std::string IControllerDungeonBase::get_type() const
    {
        return IControllerDungeonBase::TYPE;
    }

    void IControllerDungeonBase::serialize_xml(SerializerXml& serializer) const
    {

    }

    void IControllerDungeonBase::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void IControllerDungeonBase::serialize_json(SerializerJson& serializer) const
    {

    }

    void IControllerDungeonBase::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
