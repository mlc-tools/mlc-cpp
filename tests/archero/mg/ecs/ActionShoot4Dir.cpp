#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionShoot4Dir.h"
#include "BuilderBullet.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionShoot4Dir::TYPE("ActionShoot4Dir");

    ActionShoot4Dir::ActionShoot4Dir()
    {

    }

    ActionShoot4Dir::~ActionShoot4Dir()
    {
    }

    void ActionShoot4Dir::shoot()
    {
        Vector horizont(1, 0);
        Vector vertical(0, 1);
        BuilderBullet(this->entity_id).set_spine_bone_of_create_bullet("shot").set_direction(-vertical).build(model);
        BuilderBullet(this->entity_id).set_spine_bone_of_create_bullet("shot2").set_direction(horizont).build(model);
        BuilderBullet(this->entity_id).set_spine_bone_of_create_bullet("shot3").set_direction(-horizont).build(model);
        BuilderBullet(this->entity_id).set_spine_bone_of_create_bullet("shot4").set_direction(vertical).build(model);
    }

    bool ActionShoot4Dir::operator ==(const ActionShoot4Dir& rhs) const
    {
        bool result = this->ActionShoot::operator ==(rhs);
        return result;
    }

    bool ActionShoot4Dir::operator !=(const ActionShoot4Dir& rhs) const
    {
        return !(*this == rhs);
    }

    ActionShoot4Dir::ActionShoot4Dir(const ActionShoot4Dir& rhs)
    {
        this->operator=(rhs);
    }

    const ActionShoot4Dir& ActionShoot4Dir::operator =(const ActionShoot4Dir& rhs)
    {
        this->ActionShoot::operator=(rhs);
        return *this;
    }

    std::string ActionShoot4Dir::get_type() const
    {
        return ActionShoot4Dir::TYPE;
    }

    void ActionShoot4Dir::serialize_xml(SerializerXml& serializer) const
    {
        ActionShoot::serialize_xml(serializer);
    }

    void ActionShoot4Dir::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionShoot::deserialize_xml(deserializer);
    }

    void ActionShoot4Dir::serialize_json(SerializerJson& serializer) const
    {
        ActionShoot::serialize_json(serializer);
    }

    void ActionShoot4Dir::deserialize_json(DeserializerJson& deserializer)
    {
        ActionShoot::deserialize_json(deserializer);
    }

} //namespace mg
