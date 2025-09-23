#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionBase.h"
#include "ActionCreateDamage.h"
#include "BuilderDamage.h"
#include "ComponentBusy.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionCreateDamage::TYPE("ActionCreateDamage");

    ActionCreateDamage::ActionCreateDamage()
    {

    }

    ActionCreateDamage::~ActionCreateDamage()
    {
    }

    void ActionCreateDamage::on_start()
    {
        model->get<ComponentBusy>(this->entity_id)->is_busy = true;
        model->event_skill_animate[this->entity_id].notify("attack", true);
        BuilderDamage().build(model, this->entity_id, 0, model->player_id);
    }

    void ActionCreateDamage::on_finish()
    {
        model->get<ComponentBusy>(this->entity_id)->is_busy = false;
    }

    bool ActionCreateDamage::operator ==(const ActionCreateDamage& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        return result;
    }

    bool ActionCreateDamage::operator !=(const ActionCreateDamage& rhs) const
    {
        return !(*this == rhs);
    }

    ActionCreateDamage::ActionCreateDamage(const ActionCreateDamage& rhs)
    {
        this->operator=(rhs);
    }

    const ActionCreateDamage& ActionCreateDamage::operator =(const ActionCreateDamage& rhs)
    {
        this->ActionBase::operator=(rhs);
        return *this;
    }

    std::string ActionCreateDamage::get_type() const
    {
        return ActionCreateDamage::TYPE;
    }

    void ActionCreateDamage::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
    }

    void ActionCreateDamage::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
    }

    void ActionCreateDamage::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
    }

    void ActionCreateDamage::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
    }

} //namespace mg
