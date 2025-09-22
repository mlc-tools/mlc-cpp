#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentMeleeAttack.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentMeleeAttack::TYPE("ComponentMeleeAttack");

    ComponentMeleeAttack::ComponentMeleeAttack()
    {

    }

    ComponentMeleeAttack::~ComponentMeleeAttack()
    {
    }

    bool ComponentMeleeAttack::operator ==(const ComponentMeleeAttack& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        return result;
    }

    bool ComponentMeleeAttack::operator !=(const ComponentMeleeAttack& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentMeleeAttack::ComponentMeleeAttack(const ComponentMeleeAttack& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentMeleeAttack& ComponentMeleeAttack::operator =(const ComponentMeleeAttack& rhs)
    {
        this->ComponentBase::operator=(rhs);
        return *this;
    }

    std::string ComponentMeleeAttack::get_type() const
    {
        return ComponentMeleeAttack::TYPE;
    }

    void ComponentMeleeAttack::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentMeleeAttack>(this);
    }

    void ComponentMeleeAttack::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentMeleeAttack>(this);
    }

    intrusive_ptr<ComponentBase> ComponentMeleeAttack::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentMeleeAttack>(id);
    }

    void ComponentMeleeAttack::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
    }

    void ComponentMeleeAttack::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
    }

    void ComponentMeleeAttack::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
    }

    void ComponentMeleeAttack::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
    }

} //namespace mg
