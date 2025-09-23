#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionBase.h"
#include "ActionMoveDirection.h"
#include "MoveDirection.h"
#include "SystemMovement.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionMoveDirection::TYPE("ActionMoveDirection");

    ActionMoveDirection::ActionMoveDirection()
    {

    }

    ActionMoveDirection::~ActionMoveDirection()
    {
    }

    void ActionMoveDirection::on_start()
    {
        auto movement = make_intrusive<MoveDirection>();
        movement->direction = this->choose_direction();
        model->add(movement, this->entity_id);
        this->common_dictionary->vectors["move_direction"] = movement->direction;

        SystemMovement::event_on_wall[this->entity_id].add(this, [this]()
        {
            auto movement = this->model->get<MoveDirection>(this->entity_id);
            if(movement)
            {
                auto r = random_int(0, 3);
                if(r == 0)
                {
                    movement->direction.x = -movement->direction.x;
                }
                else if(r == 1)
                {
                    movement->direction.y = -movement->direction.y;
                }
                else if(r == 2)
                {
                    movement->direction = -movement->direction;
                }
            }
            else
            {
                this->_finished = true;
            }

        });
    }

    void ActionMoveDirection::on_update(float dt)
    {

    }

    void ActionMoveDirection::on_break()
    {
        model->remove(model->get<MoveDirection>(this->entity_id));
    }

    void ActionMoveDirection::on_finish()
    {
        model->remove(model->get<MoveDirection>(this->entity_id));

        map_remove(SystemMovement::event_on_wall, this->entity_id);
    }

    Vector ActionMoveDirection::choose_direction()
    {
        Vector direction;
        direction.x = random_float() * 2 - 1;
        direction.y = random_float() * 2 - 1;
        direction.normalize();
        return direction;
    }

    bool ActionMoveDirection::operator ==(const ActionMoveDirection& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        return result;
    }

    bool ActionMoveDirection::operator !=(const ActionMoveDirection& rhs) const
    {
        return !(*this == rhs);
    }

    ActionMoveDirection::ActionMoveDirection(const ActionMoveDirection& rhs)
    {
        this->operator=(rhs);
    }

    const ActionMoveDirection& ActionMoveDirection::operator =(const ActionMoveDirection& rhs)
    {
        this->ActionBase::operator=(rhs);
        return *this;
    }

    std::string ActionMoveDirection::get_type() const
    {
        return ActionMoveDirection::TYPE;
    }

    void ActionMoveDirection::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
    }

    void ActionMoveDirection::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
    }

    void ActionMoveDirection::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
    }

    void ActionMoveDirection::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
    }

} //namespace mg
