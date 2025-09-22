#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionMoveToPlayer.h"
#include "MoveToTarget.h"
#include "SystemMovement.h"
#include "SystemResolveCollisions.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionMoveToPlayer::TYPE("ActionMoveToPlayer");

    ActionMoveToPlayer::ActionMoveToPlayer()
    {

    }

    ActionMoveToPlayer::~ActionMoveToPlayer()
    {
    }

    void ActionMoveToPlayer::on_start()
    {
        auto movement = make_intrusive<MoveToTarget>();
        movement->target_id = model->player_id;
        model->add(movement, this->entity_id);
        SystemResolveCollisions::event_collision[this->entity_id].add(this, [this](int target_id)
        {
            if(target_id == model->player_id)
            {
                this->_breaked = true;
            }
        });
    }

    void ActionMoveToPlayer::on_update(float dt)
    {

    }

    void ActionMoveToPlayer::on_break()
    {
        this->finalize();
        this->set_next_action(this->next_action_on_break);
    }

    void ActionMoveToPlayer::on_finish()
    {
        this->finalize();
        this->set_next_action(this->next_action_on_finish);
    }

    void ActionMoveToPlayer::finalize()
    {
        model->remove(model->get<MoveToTarget>(this->entity_id));
        map_remove(SystemResolveCollisions::event_collision, this->entity_id);
        map_remove(SystemMovement::event_on_wall, this->entity_id);
    }

    bool ActionMoveToPlayer::operator ==(const ActionMoveToPlayer& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        return result;
    }

    bool ActionMoveToPlayer::operator !=(const ActionMoveToPlayer& rhs) const
    {
        return !(*this == rhs);
    }

    ActionMoveToPlayer::ActionMoveToPlayer(const ActionMoveToPlayer& rhs)
    {
        this->operator=(rhs);
    }

    const ActionMoveToPlayer& ActionMoveToPlayer::operator =(const ActionMoveToPlayer& rhs)
    {
        this->ActionBase::operator=(rhs);
        return *this;
    }

    std::string ActionMoveToPlayer::get_type() const
    {
        return ActionMoveToPlayer::TYPE;
    }

    void ActionMoveToPlayer::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
    }

    void ActionMoveToPlayer::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
    }

    void ActionMoveToPlayer::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
    }

    void ActionMoveToPlayer::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
    }

} //namespace mg
