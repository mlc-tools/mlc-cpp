#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentExpDrop.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentExpDrop::TYPE("ComponentExpDrop");

    ComponentExpDrop::ComponentExpDrop()
    : count(0)
    {

    }

    ComponentExpDrop::~ComponentExpDrop()
    {
    }

    bool ComponentExpDrop::operator ==(const ComponentExpDrop& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->count == rhs.count;
        return result;
    }

    bool ComponentExpDrop::operator !=(const ComponentExpDrop& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentExpDrop::ComponentExpDrop(const ComponentExpDrop& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentExpDrop& ComponentExpDrop::operator =(const ComponentExpDrop& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->count = rhs.count;
        return *this;
    }

    std::string ComponentExpDrop::get_type() const
    {
        return ComponentExpDrop::TYPE;
    }

    void ComponentExpDrop::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentExpDrop>(this);
    }

    void ComponentExpDrop::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentExpDrop>(this);
    }

    intrusive_ptr<ComponentBase> ComponentExpDrop::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentExpDrop>(id);
    }

    void ComponentExpDrop::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(count, "count", int(0));
    }

    void ComponentExpDrop::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(count, "count", int(0));
    }

    void ComponentExpDrop::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(count, "count", int(0));
    }

    void ComponentExpDrop::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(count, "count", int(0));
    }

} //namespace mg
