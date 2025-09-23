#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "EntityBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string EntityBase::TYPE("EntityBase");

    EntityBase::EntityBase()
    : id(0)
    , _reference_counter(1)
    {

    }

    EntityBase::~EntityBase()
    {
    }

    void EntityBase::retain()
    {
        ++this->_reference_counter;
    }

    int EntityBase::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool EntityBase::operator ==(const EntityBase& rhs) const
    {
        bool result = true;
        result = result && this->id == rhs.id;
        return result;
    }

    bool EntityBase::operator !=(const EntityBase& rhs) const
    {
        return !(*this == rhs);
    }

    EntityBase::EntityBase(const EntityBase& rhs)
    {
        this->operator=(rhs);
    }

    const EntityBase& EntityBase::operator =(const EntityBase& rhs)
    {
        this->id = rhs.id;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string EntityBase::get_type() const
    {
        return EntityBase::TYPE;
    }

    void EntityBase::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(id, "id", int(0));
    }

    void EntityBase::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
    }

    void EntityBase::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(id, "id", int(0));
    }

    void EntityBase::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
    }

} //namespace mg
