#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionWait.h"
#include "ComponentBusy.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionWait::TYPE("ActionWait");

    ActionWait::ActionWait()
    : animation("idle")
    , animation_loop(false)
    {

    }

    ActionWait::~ActionWait()
    {
    }

    void ActionWait::on_start()
    {
        model->get<ComponentBusy>(this->entity_id)->is_busy = true;
        this->check_duration(animation);
        this->model->event_skill_animate[this->entity_id].notify(animation, this->animation_loop);
    }

    void ActionWait::on_finish()
    {
        model->get<ComponentBusy>(this->entity_id)->is_busy = false;
    }

    bool ActionWait::operator ==(const ActionWait& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        result = result && this->animation == rhs.animation;
        result = result && this->animation_loop == rhs.animation_loop;
        return result;
    }

    bool ActionWait::operator !=(const ActionWait& rhs) const
    {
        return !(*this == rhs);
    }

    ActionWait::ActionWait(const ActionWait& rhs)
    {
        this->operator=(rhs);
    }

    const ActionWait& ActionWait::operator =(const ActionWait& rhs)
    {
        this->ActionBase::operator=(rhs);
        this->animation = rhs.animation;
        this->animation_loop = rhs.animation_loop;
        return *this;
    }

    std::string ActionWait::get_type() const
    {
        return ActionWait::TYPE;
    }

    void ActionWait::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
        serializer.serialize(animation, "animation", std::string("idle"));
        serializer.serialize(animation_loop, "animation_loop", bool(false));
    }

    void ActionWait::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
        deserializer.deserialize(animation, "animation", std::string("idle"));
        deserializer.deserialize(animation_loop, "animation_loop", bool(false));
    }

    void ActionWait::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
        serializer.serialize(animation, "animation", std::string("idle"));
        serializer.serialize(animation_loop, "animation_loop", bool(false));
    }

    void ActionWait::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
        deserializer.deserialize(animation, "animation", std::string("idle"));
        deserializer.deserialize(animation_loop, "animation_loop", bool(false));
    }

} //namespace mg
