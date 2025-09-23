#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentBase::TYPE("ComponentBase");

    ComponentBase::ComponentBase()
    : id(0)
    , _reference_counter(1)
    {

    }

    ComponentBase::~ComponentBase()
    {
    }

    void ComponentBase::retain()
    {
        ++this->_reference_counter;
    }

    int ComponentBase::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool ComponentBase::operator ==(const ComponentBase& rhs) const
    {
        bool result = true;
        result = result && this->id == rhs.id;
        return result;
    }

    bool ComponentBase::operator !=(const ComponentBase& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentBase::ComponentBase(const ComponentBase& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentBase& ComponentBase::operator =(const ComponentBase& rhs)
    {
        this->id = rhs.id;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ComponentBase::get_type() const
    {
        return ComponentBase::TYPE;
    }

    void ComponentBase::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(id, "id", int(0));
    }

    void ComponentBase::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
    }

    void ComponentBase::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(id, "id", int(0));
    }

    void ComponentBase::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
    }

} //namespace mg
