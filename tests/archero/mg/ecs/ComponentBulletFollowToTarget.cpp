#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentBullet.h"
#include "ComponentBulletFollowToTarget.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentBulletFollowToTarget::TYPE("ComponentBulletFollowToTarget");

    ComponentBulletFollowToTarget::ComponentBulletFollowToTarget()
    : target_id(0)
    , idle(0.1f)
    , change_angle_speed(1)
    {

    }

    ComponentBulletFollowToTarget::~ComponentBulletFollowToTarget()
    {
    }

    bool ComponentBulletFollowToTarget::operator ==(const ComponentBulletFollowToTarget& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->target_id == rhs.target_id;
        result = result && this->idle == rhs.idle;
        result = result && this->change_angle_speed == rhs.change_angle_speed;
        return result;
    }

    bool ComponentBulletFollowToTarget::operator !=(const ComponentBulletFollowToTarget& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentBulletFollowToTarget::ComponentBulletFollowToTarget(const ComponentBulletFollowToTarget& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentBulletFollowToTarget& ComponentBulletFollowToTarget::operator =(const ComponentBulletFollowToTarget& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->target_id = rhs.target_id;
        this->idle = rhs.idle;
        this->change_angle_speed = rhs.change_angle_speed;
        return *this;
    }

    std::string ComponentBulletFollowToTarget::get_type() const
    {
        return ComponentBulletFollowToTarget::TYPE;
    }

    void ComponentBulletFollowToTarget::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentBulletFollowToTarget>(this);
    }

    void ComponentBulletFollowToTarget::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentBulletFollowToTarget>(this);
    }

    intrusive_ptr<ComponentBase> ComponentBulletFollowToTarget::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentBulletFollowToTarget>(id);
    }

    void ComponentBulletFollowToTarget::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(idle, "idle", float(0.1f));
        serializer.serialize(change_angle_speed, "change_angle_speed", float(1));
    }

    void ComponentBulletFollowToTarget::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(idle, "idle", float(0.1f));
        deserializer.deserialize(change_angle_speed, "change_angle_speed", float(1));
    }

    void ComponentBulletFollowToTarget::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(idle, "idle", float(0.1f));
        serializer.serialize(change_angle_speed, "change_angle_speed", float(1));
    }

    void ComponentBulletFollowToTarget::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(idle, "idle", float(0.1f));
        deserializer.deserialize(change_angle_speed, "change_angle_speed", float(1));
    }

} //namespace mg
