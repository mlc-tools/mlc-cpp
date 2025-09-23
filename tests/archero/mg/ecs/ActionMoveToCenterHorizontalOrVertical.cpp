#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionMoveDirection.h"
#include "ActionMoveToCenterHorizontalOrVertical.h"
#include "MoveDirection.h"
#include "Transform.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionMoveToCenterHorizontalOrVertical::TYPE("ActionMoveToCenterHorizontalOrVertical");

    ActionMoveToCenterHorizontalOrVertical::ActionMoveToCenterHorizontalOrVertical()
    {

    }

    ActionMoveToCenterHorizontalOrVertical::~ActionMoveToCenterHorizontalOrVertical()
    {
    }

    Vector ActionMoveToCenterHorizontalOrVertical::choose_direction()
    {
        auto transform = model->get<Transform>(this->entity_id);
        auto center = model->ground->get_center_position();
        Vector direction;
        auto rand = random_int(0, 2);
        if(rand == 0)
        {
            if(transform->position.x < center.x) direction.x = 1;
            else direction.x = -1;
        }
        else
        {
            if(transform->position.y < center.y) direction.y = 1;
            else direction.y = -1;
        }
        return direction;
    }

    bool ActionMoveToCenterHorizontalOrVertical::operator ==(const ActionMoveToCenterHorizontalOrVertical& rhs) const
    {
        bool result = this->ActionMoveDirection::operator ==(rhs);
        return result;
    }

    bool ActionMoveToCenterHorizontalOrVertical::operator !=(const ActionMoveToCenterHorizontalOrVertical& rhs) const
    {
        return !(*this == rhs);
    }

    ActionMoveToCenterHorizontalOrVertical::ActionMoveToCenterHorizontalOrVertical(const ActionMoveToCenterHorizontalOrVertical& rhs)
    {
        this->operator=(rhs);
    }

    const ActionMoveToCenterHorizontalOrVertical& ActionMoveToCenterHorizontalOrVertical::operator =(const ActionMoveToCenterHorizontalOrVertical& rhs)
    {
        this->ActionMoveDirection::operator=(rhs);
        return *this;
    }

    std::string ActionMoveToCenterHorizontalOrVertical::get_type() const
    {
        return ActionMoveToCenterHorizontalOrVertical::TYPE;
    }

    void ActionMoveToCenterHorizontalOrVertical::serialize_xml(SerializerXml& serializer) const
    {
        ActionMoveDirection::serialize_xml(serializer);
    }

    void ActionMoveToCenterHorizontalOrVertical::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionMoveDirection::deserialize_xml(deserializer);
    }

    void ActionMoveToCenterHorizontalOrVertical::serialize_json(SerializerJson& serializer) const
    {
        ActionMoveDirection::serialize_json(serializer);
    }

    void ActionMoveToCenterHorizontalOrVertical::deserialize_json(DeserializerJson& deserializer)
    {
        ActionMoveDirection::deserialize_json(deserializer);
    }

} //namespace mg
