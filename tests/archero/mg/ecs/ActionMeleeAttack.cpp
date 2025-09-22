#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionMeleeAttack.h"
#include "BuilderDamage.h"
#include "ComponentBusy.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionMeleeAttack::TYPE("ActionMeleeAttack");

    ActionMeleeAttack::ActionMeleeAttack()
    {

    }

    ActionMeleeAttack::~ActionMeleeAttack()
    {
    }

    void ActionMeleeAttack::on_start()
    {
        model->get<ComponentBusy>(this->entity_id)->is_busy = true;
        model->event_skill_animate[this->entity_id].notify("attack", true);
        BuilderDamage().build(model, this->entity_id, 0, model->player_id);
    }

    void ActionMeleeAttack::on_finish()
    {
        model->get<ComponentBusy>(this->entity_id)->is_busy = false;
    }

    bool ActionMeleeAttack::operator ==(const ActionMeleeAttack& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        return result;
    }

    bool ActionMeleeAttack::operator !=(const ActionMeleeAttack& rhs) const
    {
        return !(*this == rhs);
    }

    ActionMeleeAttack::ActionMeleeAttack(const ActionMeleeAttack& rhs)
    {
        this->operator=(rhs);
    }

    const ActionMeleeAttack& ActionMeleeAttack::operator =(const ActionMeleeAttack& rhs)
    {
        this->ActionBase::operator=(rhs);
        return *this;
    }

    std::string ActionMeleeAttack::get_type() const
    {
        return ActionMeleeAttack::TYPE;
    }

    void ActionMeleeAttack::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
    }

    void ActionMeleeAttack::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
    }

    void ActionMeleeAttack::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
    }

    void ActionMeleeAttack::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
    }

} //namespace mg
