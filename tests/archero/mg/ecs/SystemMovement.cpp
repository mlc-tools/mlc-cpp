#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../Observable.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "Circle.h"
#include "CollisionChecker.h"
#include "ComponentBody.h"
#include "ComponentBullet.h"
#include "ComponentMovement.h"
#include "ComponentStats.h"
#include "ComponentStun.h"
#include "ModelEcsBase.h"
#include "MoveDirection.h"
#include "MoveInstant.h"
#include "MoveParabolic.h"
#include "MoveToTarget.h"
#include "MoveVertical.h"
#include "SystemMovement.h"
#include "SystemResolveCollisions.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include <map>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    std::map<int, Observable<void()>> SystemMovement::event_on_wall;
    const std::string SystemMovement::TYPE("SystemMovement");

    SystemMovement::SystemMovement()
    : _reference_counter(1)
    {

    }

    SystemMovement::~SystemMovement()
    {
    }

    void SystemMovement::update(ModelEcsBase* model, float dt)
    {
        this->move_by_direction(model, dt);
        this->move_to_target(model, dt);
        this->move_by_parabolic(model, dt);
        this->move_instant(model, dt);
        this->move_vertical(model, dt);
    }

    void SystemMovement::clean(ModelEcsBase* model)
    {
        model->each_if<MoveDirection, Transform>(
        [&](auto& move, auto& transform)
        {
            return !model->ground->is_bound(transform->position);
        },
        [&](auto& move, auto& transform)
        {
            move->clean = true;
        });

        for(int __index__ = 0; __index__ < model->components_move_direction.size(); ++__index__)
        {
            auto& component = model->components_move_direction.at(__index__);
            if(component->clean )
            {
                auto __size__ = model->components_move_direction.size();
                model->remove_entity(component->id);
                if(__size__ != model->components_move_direction.size()) { --__index__; }
            }
        }
    }

    void SystemMovement::move_by_direction(ModelEcsBase* model, float dt)
    {
        model->each<MoveDirection, ComponentStats, Transform>(
        [&](auto& move, auto& stats, auto& transform)
        {
            auto speed = stats->get(model, UnitStat::speed);
            auto offset = move->direction * (dt * speed);
            this->move(model, move, transform, stats, offset);
        });
    }

    void SystemMovement::move_to_target(ModelEcsBase* model, float dt)
    {
        model->each<MoveToTarget, ComponentStats, Transform>(
        [&](auto& move, auto& stats, auto& transform)
        {
            auto speed = stats->get(model, UnitStat::speed);

            if(move->timer_to_move_by_changed_direction > 0)
            {
                move->timer_to_move_by_changed_direction -= dt;
                auto offset = move->changed_direction * (dt * speed);
                this->move(model, move, transform, stats, offset);
            }
            else
            {
                auto target_transform = model->get<Transform>(move->target_id);
                if(target_transform)
                {
                    auto pos = target_transform->position + move->offset;
                    auto dir = pos - transform->position;
                    dir.normalize();
                    auto offset = dir * (dt * speed);

                    if(!this->move(model, move, transform, stats, offset))
                    {
                        move->timer_to_move_by_changed_direction = 1;
                        move->changed_direction = Vector::generate_random_point_on_radius(1) + dir.normalize();
                        move->changed_direction.normalize();
                    }
                    else
                    {
                        move->direction = dir;
                    }
                }
            }
        });
    }

    void SystemMovement::move_by_parabolic(ModelEcsBase* model, float dt)
    {
        model->each<MoveParabolic, ComponentStats, Transform>(
        [&](auto& move, auto& stats, auto& transform)
        {
            auto pos = move->update_position(dt);
            transform->position = pos;
            transform->z = move->z;
        });
    }

    void SystemMovement::move_instant(ModelEcsBase* model, float dt)
    {
        model->each<MoveInstant, Transform>(
        [&](auto& move, auto& transform)
        {
            auto transform_target = model->get<Transform>(move->target_id);
            if(transform_target)
            {
                transform->position = transform_target->position;
            }
        });
    }

    void SystemMovement::move_vertical(ModelEcsBase* model, float dt)
    {
        model->each<MoveVertical, Transform>(
        [&](auto& move, auto& transform)
        {
            move->timer += dt;
            float t = move->duration - move->timer;
            float z = move->start_z * t;
            transform->z = z;
            if(t <= 0)
            {
                move->done = true;
            }
        });
    }

    bool SystemMovement::move(ModelEcsBase* model, ComponentMovement* movement, Transform* transform, ComponentStats* stats, const Vector& offset)
    {
        if(auto stun = model->get<ComponentStun>(movement->id))
        {
            if(stun->activated)
            {
                return false;
            }
        }
        auto move_offset = offset * movement->slow_k;
        auto new_pos =  transform->position + move_offset;
        bool can = true;
        Vector wall_normal;
        bool has_wall_collision = false;

        {
            bool slide_handled = false;
            auto body = model->get<ComponentBody>(transform->id);
            float radius = body ? body->size : 20;
            for(auto& polygon : model->ground->wall_polygons)
            {
                auto collision = CollisionChecker::polygon_circle_mtv(polygon, Circle(new_pos, radius));
                if(collision.collision)
                {
                    wall_normal = collision.mtv;
                    wall_normal.normalize();
                    has_wall_collision = true;
                    auto tangent = move_offset - wall_normal * move_offset.dot(wall_normal);
                    if(tangent.length() > 0)
                    {
                        tangent.normalize(move_offset.length());
                        auto slide_pos = transform->position + tangent;
                        if(SystemResolveCollisions::can_move(model, transform->id, slide_pos, stats->get(model, UnitStat::size)))
                        {
                            transform->set(slide_pos);
                            slide_handled = true;
                            can = false;
                        }
                    }
                    break;
                }
            }
            if(!slide_handled)
            {
                can = SystemResolveCollisions::can_move(model, transform->id, new_pos, stats->get(model, UnitStat::size));
            }
            if(!can && !slide_handled)
            {
                auto offset_length = move_offset.length();
                float sx = move_offset.x < 0 ? -1 : 1;
                float sy = move_offset.y < 0 ? -1 : 1;
                if(SystemResolveCollisions::can_move(model, transform->id, Vector(transform->position.x + offset_length * sx, transform->position.y), stats->get(model, UnitStat::size)))
                {
                    transform->set(Vector(transform->position.x + offset_length * sx, transform->position.y));
                }
                else if(SystemResolveCollisions::can_move(model, transform->id, Vector(transform->position.x, transform->position.y + offset_length * sy), stats->get(model, UnitStat::size)))
                {
                    transform->set(Vector(transform->position.x, transform->position.y + offset_length * sy));
                }
            }
        }

        if(!can && in_map(transform->id, SystemMovement::event_on_wall))
        {
            SystemMovement::event_on_wall[transform->id].notify();
        }

        if(can)
        {
            transform->set(new_pos);
            return true;
        }
        else
        {
            auto bullet = model->get<ComponentBullet>(transform->id);
            if(bullet && bullet->ricochet_wall > 0 && has_wall_collision)
            {
                --bullet->ricochet_wall;
                bullet->damage *= 0.5f;
                transform->prev_position = transform->position;
                auto dir = movement->direction;
                dir = dir - wall_normal * 2 * dir.dot(wall_normal);
                dir.normalize();
                if(auto move_dir = model->get<MoveDirection>(transform->id))
                {
                    move_dir->direction = dir;
                }
                else if(auto move_to = model->get<MoveToTarget>(transform->id))
                {
                    model->remove(move_to);
                    auto md = make_intrusive<MoveDirection>();
                    md->direction = dir;
                    model->add(md, transform->id);
                }
                return true;
            }
            if(bullet)
            {
                bullet->clean = true;
            }
            return false;
        }
    }

    void SystemMovement::retain()
    {
        ++this->_reference_counter;
    }

    int SystemMovement::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemMovement::operator ==(const SystemMovement& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemMovement::operator !=(const SystemMovement& rhs) const
    {
        return !(*this == rhs);
    }

    SystemMovement::SystemMovement(const SystemMovement& rhs)
    {
        this->operator=(rhs);
    }

    const SystemMovement& SystemMovement::operator =(const SystemMovement& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemMovement::get_type() const
    {
        return SystemMovement::TYPE;
    }

    void SystemMovement::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemMovement::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemMovement::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemMovement::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
