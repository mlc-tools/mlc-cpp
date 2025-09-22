#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionShootByMoveDirection.h"
#include "BuilderBullet.h"
#include "Transform.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionShootByMoveDirection::TYPE("ActionShootByMoveDirection");

    ActionShootByMoveDirection::ActionShootByMoveDirection()
    {

    }

    ActionShootByMoveDirection::~ActionShootByMoveDirection()
    {
    }

    void ActionShootByMoveDirection::shoot()
    {
        auto direction = this->common_dictionary->vectors["move_direction"];
        auto transform = model->get<Transform>(this->entity_id);
        BuilderBullet(this->entity_id).set_spine_bone_of_create_bullet("shot").set_direction(direction).build(model);
        BuilderBullet(this->entity_id).set_spine_bone_of_create_bullet("shot2").set_direction(-direction).build(model);
    }

    bool ActionShootByMoveDirection::operator ==(const ActionShootByMoveDirection& rhs) const
    {
        bool result = this->ActionShoot::operator ==(rhs);
        return result;
    }

    bool ActionShootByMoveDirection::operator !=(const ActionShootByMoveDirection& rhs) const
    {
        return !(*this == rhs);
    }

    ActionShootByMoveDirection::ActionShootByMoveDirection(const ActionShootByMoveDirection& rhs)
    {
        this->operator=(rhs);
    }

    const ActionShootByMoveDirection& ActionShootByMoveDirection::operator =(const ActionShootByMoveDirection& rhs)
    {
        this->ActionShoot::operator=(rhs);
        return *this;
    }

    std::string ActionShootByMoveDirection::get_type() const
    {
        return ActionShootByMoveDirection::TYPE;
    }

    void ActionShootByMoveDirection::serialize_xml(SerializerXml& serializer) const
    {
        ActionShoot::serialize_xml(serializer);
    }

    void ActionShootByMoveDirection::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionShoot::deserialize_xml(deserializer);
    }

    void ActionShootByMoveDirection::serialize_json(SerializerJson& serializer) const
    {
        ActionShoot::serialize_json(serializer);
    }

    void ActionShootByMoveDirection::deserialize_json(DeserializerJson& deserializer)
    {
        ActionShoot::deserialize_json(deserializer);
    }

} //namespace mg
