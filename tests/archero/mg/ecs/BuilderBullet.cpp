#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderBullet.h"
#include "BuilderEntity.h"
#include "BulletType.h"
#include "ComponentBullet.h"
#include "ComponentBulletFollowToTarget.h"
#include "ComponentHealth.h"
#include "ComponentSide.h"
#include "ComponentSpineInfo.h"
#include "ComponentStats.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include "MoveDirection.h"
#include "MoveInstant.h"
#include "MoveParabolic.h"
#include "MoveToTarget.h"
#include "MoveVertical.h"
#include "Side.h"
#include "SystemDamage.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string BuilderBullet::TYPE("BuilderBullet");

    BuilderBullet::BuilderBullet(int shooter_id)
    : shooter_id(0)
    , target_id(0)
    , bullet_name("")
    , position_create()
    , position_to_shoot()
    , direction()
    , offset_create()
    , deviation_from_direction(0)
    , side(Side::none)
    , spine_bone_of_create_bullet("shot")
    , data(nullptr)
    , damage(0)
    , _reference_counter(1)
    {
        this->shooter_id = shooter_id;
    }

    BuilderBullet::~BuilderBullet()
    {
    }

    BuilderBullet& BuilderBullet::set_name(const std::string& value)
    {
        this->bullet_name = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_target_id(int value)
    {
        this->target_id = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_damage(float value)
    {
        this->damage = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_side(Side value)
    {
        this->side = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_position_create(const Vector& value)
    {
        this->position_create = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_position_to_shoot(const Vector& value)
    {
        this->position_to_shoot = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_direction(const Vector& value)
    {
        this->direction = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_offset_create(const Vector& value)
    {
        this->offset_create = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_deviation_from_direction(float value)
    {
        this->deviation_from_direction = value; return *this;
    }

    BuilderBullet& BuilderBullet::set_spine_bone_of_create_bullet(const std::string& value)
    {
        this->spine_bone_of_create_bullet = value; return *this;
    }

    int BuilderBullet::build(ModelEcsBase* model)
    {
        assert(this->shooter_id > 0);

        auto shooter_side = model->get<ComponentSide>(this->shooter_id);
        if(!shooter_side)
        {
            return 0;
        }

        if(this->side == Side::none)
        {
            this->side = shooter_side->side;
        }
        auto shooter_stats = model->get<ComponentStats>(this->shooter_id);
        auto bullet_name = this->get_bullet_name(model, shooter_stats);
        this->data = DataStorage::shared().get<DataUnit>(bullet_name);
        auto position_bullet = this->get_position_to_create(model);

        auto id = BuilderEntity(this->data).set_side(side).set_position(position_bullet).build(model);

        auto component_bullet = model->get<ComponentBullet>(id);
        component_bullet->shooter_id = this->shooter_id;
        component_bullet->target_id = this->target_id;
        component_bullet->data = this->data;
        component_bullet->through_walls = shooter_stats->get(model, UnitStat::bullets_through_walls);
        component_bullet->pierce = shooter_stats->get(model, UnitStat::bullets_pierce);
        component_bullet->bullet_type = this->data->bullet_type;
        auto bullet_stats = model->get<ComponentStats>(id);
        component_bullet->ricochet_bounce = shooter_stats->get(model, UnitStat::ricochet_bounce);
        component_bullet->ricochet_wall = shooter_stats->get(model, UnitStat::ricochet_wall);
        component_bullet->split_on_hit = shooter_stats->get(model, UnitStat::bullet_split) > 0;

        component_bullet->damage = this->damage;

        if(this->target_id > 0)
        {
            if(component_bullet->damage == 0)
            {
                component_bullet->damage = SystemDamage::compute_future_damage(model, this->shooter_id, id, this->target_id);
            }
            auto health_target = model->get<ComponentHealth>(this->target_id);
            if(health_target)
            {
                health_target->add_future_damage(this->shooter_id, component_bullet->damage);
            }
        }


        if(shooter_stats->get(model, UnitStat::bullet_follow_target) > 0)
        {
            auto follow = make_intrusive<ComponentBulletFollowToTarget>();
            follow->target_id = this->target_id;
            follow->change_angle_speed = shooter_stats->get(model, UnitStat::bullet_follow_target);
            model->add(follow, id);
        }

        if(data->bullet_type == BulletType::arrow || data->bullet_type == BulletType::fireball)
        {
            auto movement = make_intrusive<MoveDirection>();
            movement->direction = this->get_direction(model);
            if(deviation_from_direction != 0)
            {
                auto angle = Vector::get_angle(movement->direction);
                movement->direction = Vector::build_vector(angle + deviation_from_direction);
            }
            model->add(movement, id);
        }
        else if(auto movement = model->get<MoveParabolic>(id))
        {
            auto target_position = this->get_target_position(model);
            auto start_position = position_bullet;
            auto speed = bullet_stats->get(model, UnitStat::speed);
            movement->set(start_position, target_position, speed);
        }
        else if(data->bullet_type == BulletType::lightning)
        {
            auto movement = make_intrusive<MoveInstant>();
            movement->target_id = this->target_id;
            model->add(movement, id);

        }
        else if(data->bullet_type == BulletType::laser)
        {

        }
        else if(data->bullet_type == BulletType::meteor)
        {
            auto movement = make_intrusive<MoveVertical>();
            movement->destination = this->get_target_position(model);
            model->add(movement, id);

            model->get<Transform>(id)->z = movement->start_z;
        }
        else
        {
            assert(0);
        }

        model->event_create_bullet.notify(id, position_bullet);
        return id;
    }

    Vector BuilderBullet::get_position_to_create(ModelEcsBase* model) const
    {
        if(this->position_create != Vector::ZERO)
        {
            return this->position_create;
        }
        auto transform = model->get<Transform>(this->shooter_id);
        auto spine_self = model->get<ComponentSpineInfo>(this->shooter_id);
        auto position_bullet = transform->position + this->offset_create;
        auto __tmp_position_bullet = spine_self;

        if(__tmp_position_bullet)

        {

            position_bullet += __tmp_position_bullet->get_bone_position(spine_bone_of_create_bullet);

        }
        return position_bullet;
    }

    Vector BuilderBullet::get_target_position(ModelEcsBase* model) const
    {
        if(this->position_to_shoot != Vector::ZERO)
        {
            return this->position_to_shoot;
        }
        assert(this->target_id > 0);
        auto transform = model->get<Transform>(this->target_id);
        auto result = transform->position;
        if(this->data->bullet_type == BulletType::arrow || this->data->bullet_type == BulletType::fireball)
        {
            auto spine_target = model->get<ComponentSpineInfo>(this->target_id);
            result += spine_target->get_bone_position("head");
        }

        if(side == Side::ally)
        {
            Vector move_direction;
            if(auto movement = model->get<MoveDirection>(this->target_id))
            {
                move_direction = movement->direction;
            }
            else if(auto movement = model->get<MoveToTarget>(this->target_id))
            {
                move_direction = movement->direction;
            }
            if(move_direction != Vector::ZERO)
            {
                float bullet_speed = this->data->stats.at(UnitStat::speed);
                float target_speed = model->get<ComponentStats>(this->target_id)->get(model, UnitStat::speed);
                result = this->get_preemption(transform->position, move_direction, target_speed, this->get_position_to_create(model), bullet_speed);
            }
        }

        return result;
    }

    Vector BuilderBullet::get_preemption(const Vector& target_pos, const Vector& target_direction, float target_speed, const Vector& shooter_pos, float bullet_speed) const
    {
        Vector result = target_pos;
        Vector target_velocity = target_direction.get_normalized() * target_speed;

        Vector displacement = target_pos - shooter_pos;

        float a = target_velocity.dot(target_velocity) - bullet_speed * bullet_speed;
        float b = 2 * displacement.dot(target_velocity);
        float c = displacement.dot(displacement);

        float discriminant = b*b - 4*a*c;

        if (discriminant >= 0 && std::abs(a) > 1e-6) {
        float sqrt_disc = sqrt(discriminant);
        float t1 = (-b + sqrt_disc) / (2 * a);
        float t2 = (-b - sqrt_disc) / (2 * a);

        float t = std::min(t1, t2);
        if (t < 0) t = std::max(t1, t2);

        if (t > 0) {
        Vector intercept_point = target_pos + target_velocity * t;
        result = intercept_point;
    }
}
return result;
}

Vector BuilderBullet::get_direction(ModelEcsBase* model) const
{
    if(this->direction != Vector::ZERO)
    {
        return this->direction;
    }
    auto from_pos = this->get_position_to_create(model);
    auto to_pos = this->get_target_position(model);
    return (to_pos - from_pos).normalize();
}

std::string BuilderBullet::get_bullet_name(ModelEcsBase* model, ComponentStats* stats) const
{
    if(string_size(this->bullet_name) > 0)
    {
        return this->bullet_name;
    }
    return stats->get_string(model, UnitStat::bullet);
}

void BuilderBullet::retain()
{
    ++this->_reference_counter;
}

int BuilderBullet::release()
{
    --this->_reference_counter;
    auto counter = this->_reference_counter;
    if(counter == 0)
    {
        delete this;
    }
    return counter;
}

bool BuilderBullet::operator ==(const BuilderBullet& rhs) const
{
    bool result = true;
    result = result && this->shooter_id == rhs.shooter_id;
    result = result && this->target_id == rhs.target_id;
    result = result && this->bullet_name == rhs.bullet_name;
    result = result && this->position_create == rhs.position_create;
    result = result && this->position_to_shoot == rhs.position_to_shoot;
    result = result && this->direction == rhs.direction;
    result = result && this->offset_create == rhs.offset_create;
    result = result && this->deviation_from_direction == rhs.deviation_from_direction;
    result = result && this->side == rhs.side;
    result = result && this->spine_bone_of_create_bullet == rhs.spine_bone_of_create_bullet;
    result = result && ((this->data == rhs.data) || (this->data != nullptr && rhs.data != nullptr && *this->data == *rhs.data));
    result = result && this->damage == rhs.damage;
    return result;
}

bool BuilderBullet::operator !=(const BuilderBullet& rhs) const
{
    return !(*this == rhs);
}

BuilderBullet::BuilderBullet(const BuilderBullet& rhs)
{
    this->operator=(rhs);
}

const BuilderBullet& BuilderBullet::operator =(const BuilderBullet& rhs)
{
    this->shooter_id = rhs.shooter_id;
    this->target_id = rhs.target_id;
    this->bullet_name = rhs.bullet_name;
    this->position_create = rhs.position_create;
    this->position_to_shoot = rhs.position_to_shoot;
    this->direction = rhs.direction;
    this->offset_create = rhs.offset_create;
    this->deviation_from_direction = rhs.deviation_from_direction;
    this->side = rhs.side;
    this->spine_bone_of_create_bullet = rhs.spine_bone_of_create_bullet;
    this->data = rhs.data;
    this->damage = rhs.damage;
    this->_reference_counter = rhs._reference_counter;
    return *this;
}

std::string BuilderBullet::get_type() const
{
    return BuilderBullet::TYPE;
}

void BuilderBullet::serialize_xml(SerializerXml& serializer) const
{
    serializer.serialize(shooter_id, "shooter_id", int(0));
    serializer.serialize(target_id, "target_id", int(0));
    serializer.serialize(bullet_name, "bullet_name", std::string(""));
    serializer.serialize(position_create, "position_create");
    serializer.serialize(position_to_shoot, "position_to_shoot");
    serializer.serialize(direction, "direction");
    serializer.serialize(offset_create, "offset_create");
    serializer.serialize(deviation_from_direction, "deviation_from_direction", float(0));
    serializer.serialize(side, "side");
    serializer.serialize(spine_bone_of_create_bullet, "spine_bone_of_create_bullet", std::string("shot"));
    serializer.serialize(data, "data");
    serializer.serialize(damage, "damage", float(0));
}

void BuilderBullet::deserialize_xml(DeserializerXml& deserializer)
{
    deserializer.deserialize(shooter_id, "shooter_id", int(0));
    deserializer.deserialize(target_id, "target_id", int(0));
    deserializer.deserialize(bullet_name, "bullet_name", std::string(""));
    deserializer.deserialize(position_create, "position_create");
    deserializer.deserialize(position_to_shoot, "position_to_shoot");
    deserializer.deserialize(direction, "direction");
    deserializer.deserialize(offset_create, "offset_create");
    deserializer.deserialize(deviation_from_direction, "deviation_from_direction", float(0));
    deserializer.deserialize(side, "side");
    deserializer.deserialize(spine_bone_of_create_bullet, "spine_bone_of_create_bullet", std::string("shot"));
    deserializer.deserialize(data, "data");
    deserializer.deserialize(damage, "damage", float(0));
}

void BuilderBullet::serialize_json(SerializerJson& serializer) const
{
    serializer.serialize(shooter_id, "shooter_id", int(0));
    serializer.serialize(target_id, "target_id", int(0));
    serializer.serialize(bullet_name, "bullet_name", std::string(""));
    serializer.serialize(position_create, "position_create");
    serializer.serialize(position_to_shoot, "position_to_shoot");
    serializer.serialize(direction, "direction");
    serializer.serialize(offset_create, "offset_create");
    serializer.serialize(deviation_from_direction, "deviation_from_direction", float(0));
    serializer.serialize(side, "side");
    serializer.serialize(spine_bone_of_create_bullet, "spine_bone_of_create_bullet", std::string("shot"));
    serializer.serialize(data, "data");
    serializer.serialize(damage, "damage", float(0));
}

void BuilderBullet::deserialize_json(DeserializerJson& deserializer)
{
    deserializer.deserialize(shooter_id, "shooter_id", int(0));
    deserializer.deserialize(target_id, "target_id", int(0));
    deserializer.deserialize(bullet_name, "bullet_name", std::string(""));
    deserializer.deserialize(position_create, "position_create");
    deserializer.deserialize(position_to_shoot, "position_to_shoot");
    deserializer.deserialize(direction, "direction");
    deserializer.deserialize(offset_create, "offset_create");
    deserializer.deserialize(deviation_from_direction, "deviation_from_direction", float(0));
    deserializer.deserialize(side, "side");
    deserializer.deserialize(spine_bone_of_create_bullet, "spine_bone_of_create_bullet", std::string("shot"));
    deserializer.deserialize(data, "data");
    deserializer.deserialize(damage, "damage", float(0));
}

} //namespace mg
