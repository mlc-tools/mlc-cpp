#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentCreateMovementToHero.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentCreateMovementToHero::TYPE("ComponentCreateMovementToHero");

    ComponentCreateMovementToHero::ComponentCreateMovementToHero()
    {

    }

    ComponentCreateMovementToHero::~ComponentCreateMovementToHero()
    {
    }

    bool ComponentCreateMovementToHero::operator ==(const ComponentCreateMovementToHero& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        return result;
    }

    bool ComponentCreateMovementToHero::operator !=(const ComponentCreateMovementToHero& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentCreateMovementToHero::ComponentCreateMovementToHero(const ComponentCreateMovementToHero& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentCreateMovementToHero& ComponentCreateMovementToHero::operator =(const ComponentCreateMovementToHero& rhs)
    {
        this->ComponentBase::operator=(rhs);
        return *this;
    }

    std::string ComponentCreateMovementToHero::get_type() const
    {
        return ComponentCreateMovementToHero::TYPE;
    }

    void ComponentCreateMovementToHero::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentCreateMovementToHero>(this);
    }

    void ComponentCreateMovementToHero::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentCreateMovementToHero>(this);
    }

    intrusive_ptr<ComponentBase> ComponentCreateMovementToHero::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentCreateMovementToHero>(id);
    }

    void ComponentCreateMovementToHero::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
    }

    void ComponentCreateMovementToHero::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
    }

    void ComponentCreateMovementToHero::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
    }

    void ComponentCreateMovementToHero::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
    }

} //namespace mg
