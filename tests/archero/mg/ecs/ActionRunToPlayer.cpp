#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../model/System.h"
#include "ActionBase.h"
#include "ActionRunToPlayer.h"
#include "ComponentBusy.h"
#include "ComponentStats.h"
#include "DataStatUpgrade.h"
#include "MoveDirection.h"
#include "SystemMovement.h"
#include "SystemResolveCollisions.h"
#include "Transform.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionRunToPlayer::TYPE("ActionRunToPlayer");

    ActionRunToPlayer::ActionRunToPlayer()
    : animation("run")
    {

    }

    ActionRunToPlayer::~ActionRunToPlayer()
    {
    }

    void ActionRunToPlayer::on_start()
    {
        auto transform = model->get<Transform>(this->entity_id);
        auto transform_hero = model->get<Transform>(model->player_id);
        auto direction = transform_hero->position - transform->position;

        auto movement = make_intrusive<MoveDirection>();
        movement->direction = direction;
        movement->direction.normalize();
        model->add(movement, this->entity_id);

        auto stats = model->get<ComponentStats>(this->entity_id);
        auto buf = DataStorage::shared().get<DataStatUpgrade>("enemy_skill_run_add_speed");
        stats->add_upgrade(buf);

        model->get<ComponentBusy>(this->entity_id)->is_busy = true;
        model->event_turn_to[this->entity_id].notify(model->player_id);
        model->event_skill_animate[this->entity_id].notify(this->animation, true);

        SystemResolveCollisions::event_collision[this->entity_id].add(this, [this](int target_id)
        {
            if(target_id == model->player_id)
            {

                this->_breaked = true;
            }
        });
        SystemMovement::event_on_wall[this->entity_id].add(this, [this]()
        {
            this->_finished = true;
        });
    }

    void ActionRunToPlayer::on_update(float dt)
    {

    }

    void ActionRunToPlayer::on_break()
    {
        this->finalize();
        this->set_next_action(this->next_action_on_break);
    }

    void ActionRunToPlayer::on_finish()
    {
        this->finalize();
        this->set_next_action(this->next_action_on_finish);
    }

    void ActionRunToPlayer::finalize()
    {
        model->remove(model->get<MoveDirection>(this->entity_id));

        auto stats = model->get<ComponentStats>(this->entity_id);
        auto buf = DataStorage::shared().get<DataStatUpgrade>("enemy_skill_run_add_speed");
        stats->remove_upgrade(buf);

        model->get<ComponentBusy>(this->entity_id)->is_busy = false;

        map_remove(SystemResolveCollisions::event_collision, this->entity_id);
        map_remove(SystemMovement::event_on_wall, this->entity_id);
    }

    bool ActionRunToPlayer::operator ==(const ActionRunToPlayer& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        result = result && this->animation == rhs.animation;
        return result;
    }

    bool ActionRunToPlayer::operator !=(const ActionRunToPlayer& rhs) const
    {
        return !(*this == rhs);
    }

    ActionRunToPlayer::ActionRunToPlayer(const ActionRunToPlayer& rhs)
    {
        this->operator=(rhs);
    }

    const ActionRunToPlayer& ActionRunToPlayer::operator =(const ActionRunToPlayer& rhs)
    {
        this->ActionBase::operator=(rhs);
        this->animation = rhs.animation;
        return *this;
    }

    std::string ActionRunToPlayer::get_type() const
    {
        return ActionRunToPlayer::TYPE;
    }

    void ActionRunToPlayer::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
        serializer.serialize(animation, "animation", std::string("run"));
    }

    void ActionRunToPlayer::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
        deserializer.deserialize(animation, "animation", std::string("run"));
    }

    void ActionRunToPlayer::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
        serializer.serialize(animation, "animation", std::string("run"));
    }

    void ActionRunToPlayer::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
        deserializer.deserialize(animation, "animation", std::string("run"));
    }

} //namespace mg
