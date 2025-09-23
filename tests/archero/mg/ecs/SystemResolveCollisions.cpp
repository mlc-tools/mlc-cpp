#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../Observable.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BulletType.h"
#include "Circle.h"
#include "CollisionChecker.h"
#include "ComponentBody.h"
#include "ComponentBullet.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "Ray.h"
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
    std::map<int, Observable<void(int)>> SystemResolveCollisions::event_collision;
    const std::string SystemResolveCollisions::TYPE("SystemResolveCollisions");

    SystemResolveCollisions::SystemResolveCollisions()
    : _reference_counter(1)
    {

    }

    SystemResolveCollisions::~SystemResolveCollisions()
    {
    }

    void SystemResolveCollisions::update(ModelEcsBase* model, float dt)
    {
        this->resole_collision_with_poly(model);

        int index = 0;
        model->each<ComponentBody, Transform>(
        [&](auto& body_a, auto& transform_a)
        {
            if(transform_a->z > 0)
            {
                return;
            }
            for(int i=index+1; i<model->components_body.size(); ++i)
            {
                auto body_b = model->components_body.at(i);
                auto transform_b = model->get<Transform>(body_b->id);
                if(transform_b->z > 0)
                {
                    continue;
                }
                if(is_collision(transform_a, transform_b, body_a->size, body_b->size))
                {
                    notify(transform_a->id, transform_b->id);
                    notify(transform_b->id, transform_a->id);
                    this->resole_collision(model, transform_a, transform_b, body_a->size, body_b->size);
                }
            }
            ++index;
        }
        );
    }

    void SystemResolveCollisions::resole_collision_with_poly(ModelEcsBase* model)
    {
        model->each<ComponentBody, Transform, ComponentStats>(
        [&](auto& body, auto& transform, auto& stats)
        {
            auto size = stats->get(model, UnitStat::size);
            for(auto& polygon : model->ground->wall_polygons)
            {
                Circle circle;
                circle.center = transform->position;
                circle.radius = stats->get(model, UnitStat::size);
                auto collision = CollisionChecker::polygon_circle_mtv(polygon, circle);
                if(collision.collision)
                {
                    transform->position += collision.mtv;
                }
            }
        });
    }

    void SystemResolveCollisions::notify(int id, int target_id)
    {
        if(in_map(id, SystemResolveCollisions::event_collision))
        {
            SystemResolveCollisions::event_collision[id].notify(target_id);
        }
    }

    bool SystemResolveCollisions::is_collision(Transform* a, Transform* b, float size_a, float size_b)
    {
        bool result = a->z == 0 && b->z == 0;
        result = result && (std::fabs(a->position.x - b->position.x) + std::fabs(a->position.y - b->position.y) < (size_a+size_b));
        result = result && a->position.get_distance_sq(b->position) < (size_a+size_b)*(size_a+size_b);
        return result;
    }

    bool SystemResolveCollisions::is_collision(Transform* a, Transform* b, float size_a, float size_b, const Vector& target_offset)
    {
        return a->z == 0 && b->z == 0 && a->position.get_distance_sq(b->position + target_offset) < (size_a+size_b)*(size_a+size_b);
    }

    void SystemResolveCollisions::resole_collision(ModelEcsBase* model, Transform* transform_a, Transform* transform_b, float size_a, float size_b)
    {
        auto normal = transform_b->position - transform_a->position;
        normal.normalize(size_a + size_b);
        assert(std::fabs(normal.length() - (size_a + size_b)) < 1);
        auto pos = transform_a->position + normal;
        if(model->ground->is_bound(pos) && !model->ground->has_collision_with_walls(pos, size_b))
        {
            transform_b->position = pos;
        }
        else
        {
            transform_a->position = transform_b->position - normal;
        }
    }

    bool SystemResolveCollisions::can_move(ModelEcsBase* model, int entity_id, const Vector& pos, float radius)
    {
        auto bullet = model->get<ComponentBullet>(entity_id);
        auto bullet_type = bullet ? bullet->bullet_type : BulletType::none;

        if(bullet_type == BulletType::none)
        {
            return !is_wall(model, entity_id, pos, radius);
        }
        else if(bullet_type == BulletType::arrow)
        {
            return bullet->through_walls || !is_wall(model, entity_id, pos, radius);
        }
        else if(bullet_type == BulletType::fireball)
        {
            return true;
        }
        else if(bullet_type == BulletType::balistic)
        {
            return true;
        }
        else
        {
            assert(0);
        }
        return true;
    }

    bool SystemResolveCollisions::is_wall(ModelEcsBase* model, int entity_id, const Vector& pos, float radius)
    {
        auto is_wall = model->ground->has_collision_with_walls(pos, radius);
        return is_wall;
    }

    Vector SystemResolveCollisions::find_collision_ray_and_walls(ModelEcsBase* model, const Vector& ray_pos, const Vector& ray_direction, float min_distance)
    {
        Vector result;
        float min_dist = 99999;
        for(auto& polygon : model->ground->wall_polygons)
        {
            if(CollisionChecker::is_point_inside_polygon(ray_pos, polygon))
            {
                continue;
            }
            Ray ray(ray_pos, ray_direction);
            Vector point;
            if(!CollisionChecker::has_collision(ray, polygon, point))
            {
                continue;
            }
            auto len = (point-ray_pos).length();
            if(len < min_distance)
            {
                continue;
            }
            if(len < min_dist)
            {
                min_dist = len;
                result = point;
            }
        }
        return result;
    }

    void SystemResolveCollisions::retain()
    {
        ++this->_reference_counter;
    }

    int SystemResolveCollisions::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemResolveCollisions::operator ==(const SystemResolveCollisions& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemResolveCollisions::operator !=(const SystemResolveCollisions& rhs) const
    {
        return !(*this == rhs);
    }

    SystemResolveCollisions::SystemResolveCollisions(const SystemResolveCollisions& rhs)
    {
        this->operator=(rhs);
    }

    const SystemResolveCollisions& SystemResolveCollisions::operator =(const SystemResolveCollisions& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemResolveCollisions::get_type() const
    {
        return SystemResolveCollisions::TYPE;
    }

    void SystemResolveCollisions::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemResolveCollisions::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemResolveCollisions::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemResolveCollisions::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
