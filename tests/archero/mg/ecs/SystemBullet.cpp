#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderBullet.h"
#include "BuilderDamage.h"
#include "Circle.h"
#include "CollisionChecker.h"
#include "ComponentBullet.h"
#include "ComponentBulletLaser.h"
#include "ComponentBulletSplit.h"
#include "ComponentHealth.h"
#include "ComponentPush.h"
#include "ComponentSide.h"
#include "ComponentSpineInfo.h"
#include "ComponentStats.h"
#include "ComponentTargetHighlight.h"
#include "ComponentTargetable.h"
#include "ModelEcsBase.h"
#include "MoveDirection.h"
#include "MoveParabolic.h"
#include "MoveToTarget.h"
#include "MoveVertical.h"
#include "Side.h"
#include "SystemBullet.h"
#include "SystemDamage.h"
#include "SystemResolveCollisions.h"
#include "SystemShoot.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemBullet::TYPE("SystemBullet");

    SystemBullet::SystemBullet()
    : _reference_counter(1)
    {

    }

    SystemBullet::~SystemBullet()
    {
    }

    void SystemBullet::update(ModelEcsBase* model, float dt)
    {
        model->each<ComponentBullet, Transform, ComponentStats, ComponentSide>(
        [&](auto& bullet, auto& transform, auto& stats, auto& side)
        {
            this->update_bullet(model, bullet, transform, stats, side, dt);
        });
        this->update_split(model, dt);
        this->update_lasers(model, dt);
    }

    void SystemBullet::update_bullet(ModelEcsBase* model, ComponentBullet* component, Transform* transform, ComponentStats* bullet_stats, ComponentSide* side_bullet, float dt)
    {
        if(component->ignore_target_timer > 0)
        {
            component->ignore_target_timer -= dt;
            if(component->ignore_target_timer <= 0)
            {
                component->ignore_target_id = -1;
            }
        }

        if(auto movement = model->get<MoveToTarget>(component->id))
        {
            if(!model->get<ComponentHealth>(movement->target_id))
            {
                auto move_direction = make_intrusive<MoveDirection>();
                move_direction->direction = movement->direction;
                if(move_direction->direction == Vector::ZERO)
                {
                    move_direction->direction = Vector(1, 0);
                }
                model->add(move_direction, movement->id);

                model->remove(movement);
                return;
            }
        }

        auto move_parabolic = model->get<MoveParabolic>(component->id);
        auto move_vertical = model->get<MoveVertical>(component->id);
        auto push_speed = bullet_stats->get(model, UnitStat::push_target);
        float bullet_size = bullet_stats->get(model, UnitStat::size);
        if(bullet_size == 0)
        {
            bullet_size = 20;
        }

        if(!move_parabolic && !move_vertical)
        {
            model->each_if<ComponentSide, Transform, ComponentStats, ComponentTargetable, ComponentSpineInfo>(
            [&](auto& component_side, auto& target_transform, auto& component_stats, auto& component_targetable, auto& spine)
            {
                if(component_side->id == component->ignore_target_id)
                {
                    return false;
                }
                if(side_bullet->is_enemy(component_side->side))
                {
                    float size = component_stats->get(model, UnitStat::size);
                    auto result = SystemResolveCollisions().is_collision(transform, target_transform, size, bullet_size);
                    if(!result)
                    {
                        auto head = spine->get_bone_position("head");
                        result = SystemResolveCollisions().is_collision(transform, target_transform, size, bullet_size, head);
                    }
                    return result;
                }
                return false;
            },
            [&](auto& component_side, auto& target_transform, auto& component_stats, auto& component_targetable, auto& spine)
            {
                if(!component->clean)
                {
                    this->damage(model, component, component_targetable->id);

                    if(push_speed > 0)
                    {
                        auto push = make_intrusive<ComponentPush>();
                        push->speed = push_speed;
                        push->direction = transform->position - transform->prev_position;
                        push->direction.normalize();
                        model->add(push, component_targetable->id);
                    }

                    if(auto health_prev = model->get<ComponentHealth>(component_targetable->id))
                    {
                        health_prev->remove_future_damage(component->shooter_id);
                    }

                    if(this->try_ricochet(model, component, side_bullet, transform, component_targetable->id))
                    {
                        return;
                    }
                    this->split_on_hit(model, component, transform, component_targetable->id);
                    if(component->pierce)
                    {
                        component->ignore_target_id = component_targetable->id;
                    }
                    else if(component->always_life)
                    {
                        component->ignore_target_id = component_targetable->id;
                        component->ignore_target_timer = 1;
                    }
                    else
                    {
                        component->clean = true;
                    }
                }
            });
        }
        else if(move_parabolic && move_parabolic->done)
        {
            this->damage_massive(model, component);
            component->clean = true;
        }
        else if(move_vertical && move_vertical->done)
        {
            this->damage_massive(model, component);
            component->clean = true;
        }
    }

    void SystemBullet::update_split(ModelEcsBase* model, float dt)
    {
        auto player_transform = model->get<Transform>(model->player_id);
        model->each_if<ComponentBulletSplit, Transform, ComponentBullet, MoveDirection>(
        [&](auto& component_split, auto& transform, auto& bullet, auto& move)
        {
            return !component_split->was_split;
        },
        [&](auto& component_split, auto& transform, auto& bullet, auto& move)
        {
            if(component_split->split_near_target_radius > 0)
            {
                auto target_transform = player_transform;
                if(bullet->shooter_id == model->player_id)
                {
                    auto component_target = model->get<ComponentTargetHighlight>(model->player_id);
                    target_transform = model->get<Transform>(component_target->target_id);
                }
                assert(target_transform);
                auto distance = target_transform->position.get_distance_sq(transform->position);
                if(distance <= component_split->split_near_target_radius * component_split->split_near_target_radius)
                {
                    SystemBullet::split_bullet(model, bullet, component_split);
                }
            }

            if(!component_split->was_split && !model->ground->is_bound(transform->position + move->direction * 50))
            {
                SystemBullet::split_bullet(model, bullet, component_split);
            }
            if(!component_split->was_split && component_split->timer_to_split > 0 && (component_split->timer_to_split -= dt) <= 0)
            {
                SystemBullet::split_bullet(model, bullet, component_split);
            }
        });
    }

    void SystemBullet::update_lasers(ModelEcsBase* model, float dt)
    {
        model->each<ComponentBulletLaser, Transform, ComponentSide>([&]
        (auto& bullet, auto& transform, auto& side_bullet)
        {
            auto target_transform = model->get<Transform>(bullet->target_id);
            auto target_spine = model->get<ComponentSpineInfo>(bullet->target_id);
            Vector head_pos;
            if(target_transform)
            {
                head_pos = target_spine->get_bone_position("head");
                bullet->segment.end = target_transform->position + head_pos;
            }
            auto shooter_transform = model->get<Transform>(bullet->shooter_id);
            auto shooter_spine = model->get<ComponentSpineInfo>(bullet->shooter_id);
            if(shooter_transform && shooter_spine)
            {
                transform->set(shooter_transform->position + shooter_spine->get_bone_position("shot"));
                bullet->segment.start = transform->position;
            }

            auto dir = (bullet->segment.end - transform->position).normalize();
            auto end_pos = SystemResolveCollisions::find_collision_ray_and_walls(model, bullet->segment.start, dir, 200);
            if(end_pos != Vector::ZERO)
            {
                bullet->segment.end = end_pos;
            }

            model->each_if<ComponentSide, Transform, ComponentStats, ComponentTargetable, ComponentSpineInfo>(
            [&](auto& component_side, auto& target_transform, auto& component_stats, auto& component_targetable, auto& spine)
            {
                if(side_bullet->is_enemy(component_side->side))
                {
                    float size = component_stats->get(model, UnitStat::size);
                    auto result = CollisionChecker::has_collision(bullet->segment, Circle(target_transform->position + head_pos, size));
                    return result;
                }
                return false;
            },
            [&](auto& component_side, auto& target_transform, auto& component_stats, auto& component_targetable, auto& spine)
            {
                this->damage(model, bullet, component_targetable->id, dt / 0.3f);
            });

            bullet->duration -= dt;
        });
    }

    bool SystemBullet::try_ricochet(ModelEcsBase* model, ComponentBullet* component, ComponentSide* side_bullet, Transform* transform, int prev_target_id)
    {
        if(component->ricochet_bounce <= 0)
        {
            return false;
        }
        auto new_target = SystemShoot::get_nearest_entity_in_radius(model, component->id, 1000, side_bullet, transform, true, prev_target_id);
        if(new_target > 0 && new_target != prev_target_id)
        {
            --component->ricochet_bounce;
            component->damage *= 0.5f;
            component->target_id = new_target;
            component->ignore_target_id = prev_target_id;
            transform->prev_position = transform->position;
            if(auto target_tr = model->get<Transform>(new_target))
            {
                auto dir = (target_tr->position - transform->position).normalize();
                if(auto move_dir = model->get<MoveDirection>(component->id))
                {
                    move_dir->direction = dir;
                }
                else if(auto move_to = model->get<MoveToTarget>(component->id))
                {
                    move_to->target_id = new_target;
                    move_to->direction = dir;
                }
            }
            if(auto health_new = model->get<ComponentHealth>(new_target))
            {
                health_new->add_future_damage(component->shooter_id, component->damage);
            }
            return true;
        }
        return false;
    }

    void SystemBullet::split_on_hit(ModelEcsBase* model, ComponentBullet* bullet, Transform* transform, int target_id)
    {
        if(!bullet->split_on_hit)
        {
            return;
        }
        auto direction = transform->position - transform->prev_position;
        direction.normalize();
        auto base_angle = Vector::get_angle(direction);
        auto angle1 = base_angle + 45.f * M_PI / 180.f;
        auto angle2 = base_angle - 45.f * M_PI / 180.f;

        float base_damage = bullet->damage;
        if(base_damage <= 0)
        {
            base_damage = SystemDamage::compute_damage(model, bullet->shooter_id, target_id);
        }

        auto id1 = BuilderBullet(bullet->shooter_id).set_name(bullet->data->name).set_direction(Vector::build_vector(angle1)).set_position_create(transform->position).build(model);
        auto b1 = model->get<ComponentBullet>(id1);
        b1->damage = base_damage * 0.25f;
        b1->split_on_hit = false;
        b1->ignore_target_id = target_id;

        auto id2 = BuilderBullet(bullet->shooter_id).set_name(bullet->data->name).set_direction(Vector::build_vector(angle2)).set_position_create(transform->position).build(model);
        auto b2 = model->get<ComponentBullet>(id2);
        b2->damage = base_damage * 0.25f;
        b2->split_on_hit = false;
        b2->ignore_target_id = target_id;
    }

    void SystemBullet::damage(ModelEcsBase* model, ComponentBullet* component, int target_id, float mul)
    {
        BuilderDamage().set_damage(component->damage * mul).build(model, component->shooter_id, component->id, target_id);
    }

    void SystemBullet::damage_massive(ModelEcsBase* model, ComponentBullet* component)
    {
        auto stats = model->get<ComponentStats>(component->id);
        auto transform = model->get<Transform>(component->id);
        auto radius = stats->get(model, UnitStat::damage_mass_radius);
        auto side = model->get<ComponentSide>(component->id)->side;

        model->each_if<Transform, ComponentSide, ComponentTargetable>(
        [&](auto& target_transform, auto& component_side, auto& targetable)
        {
            return component_side->is_enemy(side) && target_transform->position.get_distance_sq(transform->position) < radius* radius;
        },
        [&](auto& target_transform, auto& component_side, auto& targetable)
        {
            BuilderDamage().build(model, component->shooter_id, component->id, target_transform->id);
        }
        );
    }

    void SystemBullet::remove_bullet(ModelEcsBase* model, ComponentBullet* bullet)
    {
        auto health = model->get<ComponentHealth>(bullet->target_id);
        if(health)
        {
            health->remove_future_damage(bullet->shooter_id);
        }


        auto component_split = model->get<ComponentBulletSplit>(bullet->id);
        if(component_split && !component_split->was_split && component_split->split_on_remove)
        {
            SystemBullet::split_bullet(model, bullet, component_split);
        }
        model->remove_entity(bullet->id);
    }

    void SystemBullet::split_bullet(ModelEcsBase* model, ComponentBullet* bullet, ComponentBulletSplit* component_split)
    {
        auto transform = model->get<Transform>(component_split->id);
        auto directions = Vector::get_points_on_radius(1, component_split->count);
        for(auto& direction : directions)
        {
            BuilderBullet(bullet->shooter_id).set_name(component_split->to_bullets->name).set_direction(direction).set_position_create(transform->position).build(model);
        }
        component_split->was_split = true;
        bullet->clean = true;
    }

    void SystemBullet::retain()
    {
        ++this->_reference_counter;
    }

    int SystemBullet::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemBullet::operator ==(const SystemBullet& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemBullet::operator !=(const SystemBullet& rhs) const
    {
        return !(*this == rhs);
    }

    SystemBullet::SystemBullet(const SystemBullet& rhs)
    {
        this->operator=(rhs);
    }

    const SystemBullet& SystemBullet::operator =(const SystemBullet& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemBullet::get_type() const
    {
        return SystemBullet::TYPE;
    }

    void SystemBullet::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemBullet::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemBullet::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemBullet::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
