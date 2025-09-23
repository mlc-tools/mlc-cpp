#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionBase.h"
#include "ActionShoot.h"
#include "BuilderBullet.h"
#include "ComponentBusy.h"
#include "ComponentSpineInfo.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionShoot::TYPE("ActionShoot");

    ActionShoot::ActionShoot()
    : animation("attack")
    , marker_id("marker")
    , bullet_name("")
    , was_shoot(false)
    , time_shoot(0)
    {

    }

    ActionShoot::~ActionShoot()
    {
    }

    void ActionShoot::on_start()
    {
        this->check_duration(animation);
        this->was_shoot = false;
        if(this->time_shoot == 0)
        {
            auto spine = model->get<ComponentSpineInfo>(this->entity_id);
            this->time_shoot = spine->get_event_time(animation, "shoot");
        }
        model->get<ComponentBusy>(this->entity_id)->is_busy = true;
        model->event_turn_to[this->entity_id].notify(model->player_id);
        model->event_skill_animate[this->entity_id].notify(animation, true);
    }

    void ActionShoot::on_finish()
    {
        model->get<ComponentBusy>(this->entity_id)->is_busy = false;
    }

    void ActionShoot::on_update(float dt)
    {
        if(!this->was_shoot && this->timer >= this->time_shoot)
        {
            this->was_shoot = true;
            this->shoot();
        }
    }

    void ActionShoot::shoot()
    {
        auto pos = this->common_dictionary->vectors[this->marker_id];
        BuilderBullet(this->entity_id).set_name(this->bullet_name).set_target_id(model->player_id).set_position_to_shoot(pos).build(model);
    }

    bool ActionShoot::operator ==(const ActionShoot& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        result = result && this->animation == rhs.animation;
        result = result && this->marker_id == rhs.marker_id;
        result = result && this->bullet_name == rhs.bullet_name;
        result = result && this->was_shoot == rhs.was_shoot;
        result = result && this->time_shoot == rhs.time_shoot;
        return result;
    }

    bool ActionShoot::operator !=(const ActionShoot& rhs) const
    {
        return !(*this == rhs);
    }

    ActionShoot::ActionShoot(const ActionShoot& rhs)
    {
        this->operator=(rhs);
    }

    const ActionShoot& ActionShoot::operator =(const ActionShoot& rhs)
    {
        this->ActionBase::operator=(rhs);
        this->animation = rhs.animation;
        this->marker_id = rhs.marker_id;
        this->bullet_name = rhs.bullet_name;
        this->was_shoot = rhs.was_shoot;
        this->time_shoot = rhs.time_shoot;
        return *this;
    }

    std::string ActionShoot::get_type() const
    {
        return ActionShoot::TYPE;
    }

    void ActionShoot::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
        serializer.serialize(animation, "animation", std::string("attack"));
        serializer.serialize(marker_id, "marker_id", std::string("marker"));
        serializer.serialize(bullet_name, "bullet_name", std::string(""));
        serializer.serialize(was_shoot, "was_shoot", bool(false));
        serializer.serialize(time_shoot, "time_shoot", float(0));
    }

    void ActionShoot::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
        deserializer.deserialize(animation, "animation", std::string("attack"));
        deserializer.deserialize(marker_id, "marker_id", std::string("marker"));
        deserializer.deserialize(bullet_name, "bullet_name", std::string(""));
        deserializer.deserialize(was_shoot, "was_shoot", bool(false));
        deserializer.deserialize(time_shoot, "time_shoot", float(0));
    }

    void ActionShoot::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
        serializer.serialize(animation, "animation", std::string("attack"));
        serializer.serialize(marker_id, "marker_id", std::string("marker"));
        serializer.serialize(bullet_name, "bullet_name", std::string(""));
        serializer.serialize(was_shoot, "was_shoot", bool(false));
        serializer.serialize(time_shoot, "time_shoot", float(0));
    }

    void ActionShoot::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
        deserializer.deserialize(animation, "animation", std::string("attack"));
        deserializer.deserialize(marker_id, "marker_id", std::string("marker"));
        deserializer.deserialize(bullet_name, "bullet_name", std::string(""));
        deserializer.deserialize(was_shoot, "was_shoot", bool(false));
        deserializer.deserialize(time_shoot, "time_shoot", float(0));
    }

} //namespace mg
