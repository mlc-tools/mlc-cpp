#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBullet.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentBullet::TYPE("ComponentBullet");

    ComponentBullet::ComponentBullet()
    : shooter_id(0)
    , target_id(0)
    , damage(0.0)
    , data(nullptr)
    , clean(false)
    , through_walls(false)
    , pierce(false)
    , bullet_type(BulletType::arrow)
    , ricochet_bounce(0)
    , ricochet_wall(0)
    , split_on_hit(false)
    , ignore_target_id(-1)
    , ignore_target_timer(0)
    , always_life(false)
    {

    }

    ComponentBullet::~ComponentBullet()
    {
    }

    bool ComponentBullet::operator ==(const ComponentBullet& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->shooter_id == rhs.shooter_id;
        result = result && this->target_id == rhs.target_id;
        result = result && this->damage == rhs.damage;
        result = result && this->clean == rhs.clean;
        result = result && this->through_walls == rhs.through_walls;
        result = result && this->pierce == rhs.pierce;
        result = result && this->bullet_type == rhs.bullet_type;
        result = result && this->ricochet_bounce == rhs.ricochet_bounce;
        result = result && this->ricochet_wall == rhs.ricochet_wall;
        result = result && this->split_on_hit == rhs.split_on_hit;
        result = result && this->ignore_target_id == rhs.ignore_target_id;
        result = result && this->ignore_target_timer == rhs.ignore_target_timer;
        result = result && this->always_life == rhs.always_life;
        return result;
    }

    bool ComponentBullet::operator !=(const ComponentBullet& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentBullet::ComponentBullet(const ComponentBullet& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentBullet& ComponentBullet::operator =(const ComponentBullet& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->shooter_id = rhs.shooter_id;
        this->target_id = rhs.target_id;
        this->damage = rhs.damage;
        this->data = rhs.data;
        this->clean = rhs.clean;
        this->through_walls = rhs.through_walls;
        this->pierce = rhs.pierce;
        this->bullet_type = rhs.bullet_type;
        this->ricochet_bounce = rhs.ricochet_bounce;
        this->ricochet_wall = rhs.ricochet_wall;
        this->split_on_hit = rhs.split_on_hit;
        this->ignore_target_id = rhs.ignore_target_id;
        this->ignore_target_timer = rhs.ignore_target_timer;
        this->always_life = rhs.always_life;
        return *this;
    }

    std::string ComponentBullet::get_type() const
    {
        return ComponentBullet::TYPE;
    }

    void ComponentBullet::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentBullet>(this);
    }

    void ComponentBullet::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentBullet>(this);
    }

    intrusive_ptr<ComponentBase> ComponentBullet::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentBullet>(id);
    }

    void ComponentBullet::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(shooter_id, "shooter_id", int(0));
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(damage, "damage", float(0.0));
        serializer.serialize(data, "data");
        serializer.serialize(clean, "clean", bool(false));
        serializer.serialize(through_walls, "through_walls", bool(false));
        serializer.serialize(pierce, "pierce", bool(false));
        serializer.serialize(bullet_type, "bullet_type");
        serializer.serialize(ricochet_bounce, "ricochet_bounce", int(0));
        serializer.serialize(ricochet_wall, "ricochet_wall", int(0));
        serializer.serialize(split_on_hit, "split_on_hit", bool(false));
        serializer.serialize(ignore_target_id, "ignore_target_id", int(-1));
        serializer.serialize(ignore_target_timer, "ignore_target_timer", float(0));
        serializer.serialize(always_life, "always_life", bool(false));
    }

    void ComponentBullet::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(shooter_id, "shooter_id", int(0));
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(damage, "damage", float(0.0));
        deserializer.deserialize(data, "data");
        deserializer.deserialize(clean, "clean", bool(false));
        deserializer.deserialize(through_walls, "through_walls", bool(false));
        deserializer.deserialize(pierce, "pierce", bool(false));
        deserializer.deserialize(bullet_type, "bullet_type");
        deserializer.deserialize(ricochet_bounce, "ricochet_bounce", int(0));
        deserializer.deserialize(ricochet_wall, "ricochet_wall", int(0));
        deserializer.deserialize(split_on_hit, "split_on_hit", bool(false));
        deserializer.deserialize(ignore_target_id, "ignore_target_id", int(-1));
        deserializer.deserialize(ignore_target_timer, "ignore_target_timer", float(0));
        deserializer.deserialize(always_life, "always_life", bool(false));
    }

    void ComponentBullet::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(shooter_id, "shooter_id", int(0));
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(damage, "damage", float(0.0));
        serializer.serialize(data, "data");
        serializer.serialize(clean, "clean", bool(false));
        serializer.serialize(through_walls, "through_walls", bool(false));
        serializer.serialize(pierce, "pierce", bool(false));
        serializer.serialize(bullet_type, "bullet_type");
        serializer.serialize(ricochet_bounce, "ricochet_bounce", int(0));
        serializer.serialize(ricochet_wall, "ricochet_wall", int(0));
        serializer.serialize(split_on_hit, "split_on_hit", bool(false));
        serializer.serialize(ignore_target_id, "ignore_target_id", int(-1));
        serializer.serialize(ignore_target_timer, "ignore_target_timer", float(0));
        serializer.serialize(always_life, "always_life", bool(false));
    }

    void ComponentBullet::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(shooter_id, "shooter_id", int(0));
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(damage, "damage", float(0.0));
        deserializer.deserialize(data, "data");
        deserializer.deserialize(clean, "clean", bool(false));
        deserializer.deserialize(through_walls, "through_walls", bool(false));
        deserializer.deserialize(pierce, "pierce", bool(false));
        deserializer.deserialize(bullet_type, "bullet_type");
        deserializer.deserialize(ricochet_bounce, "ricochet_bounce", int(0));
        deserializer.deserialize(ricochet_wall, "ricochet_wall", int(0));
        deserializer.deserialize(split_on_hit, "split_on_hit", bool(false));
        deserializer.deserialize(ignore_target_id, "ignore_target_id", int(-1));
        deserializer.deserialize(ignore_target_timer, "ignore_target_timer", float(0));
        deserializer.deserialize(always_life, "always_life", bool(false));
    }

} //namespace mg
