#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderDamage.h"
#include "ComponentHealth.h"
#include "ComponentSide.h"
#include "ComponentStats.h"
#include "ComponentSword.h"
#include "ComponentSwordCast.h"
#include "ComponentSwordState.h"
#include "Math.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "SystemDamage.h"
#include "SystemShoot.h"
#include "SystemSwords.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemSwords::TYPE("SystemSwords");

    SystemSwords::SystemSwords()
    : _reference_counter(1)
    {

    }

    SystemSwords::~SystemSwords()
    {
    }

    void SystemSwords::update(ModelEcsBase* model, float dt)
    {
        auto sword_cast = model->get<ComponentSwordCast>(model->player_id);
        auto player_transform = model->get<Transform>(model->player_id);
        this->position_swords(model, sword_cast, player_transform);
        model->each<ComponentSword, Transform, ComponentStats, ComponentSide>(
        [&](auto& sword, auto& transform, auto& stats, auto& side)
        {
            sword->timer -= dt;
            if(sword->state == ComponentSwordState::appearance)
            {
                this->appearance(model, sword, transform, sword_cast, player_transform);

                if(sword->timer <= 0)
                {
                    auto target_id = this->find_target(model, sword, transform, stats, side);
                    if(target_id > 0)
                    {
                        sword->state = ComponentSwordState::prepare_to_fly;
                        sword->target_id = target_id;
                        sword->target_pos = model->get<Transform>(target_id)->position;
                        sword->timer = sword->prepare_to_fly_duration;
                        sword->prepare_fly_start_position = transform->position;

                        auto health_target = model->get<ComponentHealth>(target_id);
                        if(health_target)
                        {
                            auto damage = SystemDamage::compute_future_damage(model, sword->id, -1, target_id);
                            health_target->add_future_damage(sword->id, damage);
                        }
                    }
                }
            }
            else if(sword->state == ComponentSwordState::prepare_to_fly)
            {
                this->prepare_to_fly(model, sword, transform);
                if(sword->timer <= 0)
                {
                    sword->state = ComponentSwordState::fly;
                    auto duration = (transform->position - sword->target_pos).length() / sword->fly_speed;
                    sword->timer = duration;
                    sword->fly_duration = duration;
                    sword->fly_start_position = transform->position;
                }
            }
            else if(sword->state == ComponentSwordState::fly)
            {
                this->fly(model, sword, transform);
                if(sword->timer <= -0.5)
                {
                    sword->clean = true;
                }
            }

            if(sword->state != ComponentSwordState::fly && sword->pos_destination != transform->position)
            {
                auto offset = sword->pos_destination - transform->position;
                auto speed = 350;
                auto len = offset.length();
                auto t = std::min(len, speed * dt);
                auto pos = transform->position + offset.normalize(t);
                transform->set(pos);
            }
        });
    }

    void SystemSwords::position_swords(ModelEcsBase* model, ComponentSwordCast* sword_cast, Transform* player_transform)
    {
        int idle_swords = 0;
        for(auto& component : model->components_sword)
        {
            if(component->state == ComponentSwordState::appearance)
            {
                idle_swords += 1;
            }
        }
        if(sword_cast->points.size() != idle_swords)
        {
            sword_cast->points = this->generate_base_points(model, idle_swords);
        }

        int index = 0;
        for(auto& component : model->components_sword)
        {
            if(component->state == ComponentSwordState::appearance)
            {
                component->player_offset = sword_cast->points[index];
                ++index;
            }
        }
    }

    std::vector<Vector> SystemSwords::generate_base_points(ModelEcsBase* model, int count)
    {
        float base_angle = 270;
        float angle_on_one_spirit = 60;
        if(count > 300 / angle_on_one_spirit)
        {
            angle_on_one_spirit = 300 / count;
        }
        float half = (count - 1) / 2.f;
        float start_angle = base_angle - angle_on_one_spirit * half;
        std::vector<Vector> points;
        for(int i=0; i<count; ++i)
        {
            points.push_back(Vector::build_vector(start_angle * M_PI / 180) * 90);
            start_angle += angle_on_one_spirit;
        }
        return points;
    }

    void SystemSwords::appearance(ModelEcsBase* model, ComponentSword* sword, Transform* transform, ComponentSwordCast* sword_cast, Transform* player_transform)
    {
        if(sword->timer <= 0)
        {
            sword->pos_destination = player_transform->position + sword->player_offset;
        }

        else
        {
            auto t = 1.f - sword->timer / sword->appearance_duration;
            t = Math::ease_back_out(t);
            auto pos = player_transform->position + sword->player_offset * t;
            sword->pos_destination = pos;
        }
    }

    int SystemSwords::find_target(ModelEcsBase* model, ComponentSword* sword, Transform* transform, ComponentStats* stats, ComponentSide* side)
    {
        auto radius = stats->get(model, UnitStat::shoot_radius);
        auto target = SystemShoot::get_nearest_entity_in_radius(model, sword->id, radius, side, transform);
        return target;
    }

    void SystemSwords::prepare_to_fly(ModelEcsBase* model, ComponentSword* sword, Transform* transform)
    {
        auto transform_target = model->get<Transform>(sword->target_id);
        auto target_position = transform_target ? transform_target->position : sword->target_pos;
        auto center = sword->prepare_fly_start_position;
        auto offset_to_target = target_position - center;
        offset_to_target.normalize();

        auto normal = Vector::get_normal_to(offset_to_target);
        if((sword->player_offset.x > 0 && normal.x < 0) || (sword->player_offset.x < 0 && normal.x > 0))
        {
            normal = -normal;
        }

        auto t = 1.f - sword->timer / sword->prepare_to_fly_duration;
        t = Math::ease_in_out(t);

        auto offset = normal * (100 * t) - offset_to_target * (sin(M_PI * t) * 50);

        auto pos = center + offset;
        sword->pos_destination = pos;
    }

    void SystemSwords::fly(ModelEcsBase* model, ComponentSword* sword, Transform* transform)
    {
        auto t = 1.f - sword->timer / sword->fly_duration;
        auto target_pos = sword->target_pos;
        auto target_transform = model->get<Transform>(sword->target_id);
        if(target_transform)
        {
            target_pos = target_transform->position;
        }
        auto dest = target_pos - sword->fly_start_position;
        auto pos = sword->fly_start_position + dest * t;
        transform->set(pos);
        if(t >= 0.9f)
        {
            if(target_transform && this->check_damage(model, sword, transform, target_transform))
            {
                this->damage(model, sword, sword->target_id);
            }
            else
            {
                auto target_id = SystemShoot::get_nearest_entity_in_radius(model, sword->id, 40, model->get<ComponentSide>(sword->id), transform);
                if(target_id)
                {
                    this->damage(model, sword, target_id);
                }
            }
        }
    }

    bool SystemSwords::check_damage(ModelEcsBase* model, ComponentSword* sword, Transform* transform, Transform* target_transform)
    {
        if(target_transform)
        {
            auto dist_sq = target_transform->position.get_distance_sq(transform->position);
            auto size = model->get<ComponentStats>(sword->target_id)->get(model, UnitStat::size);
            if(dist_sq < size * size)
            {
                return true;
            }
        }
        return false;
    }

    void SystemSwords::damage(ModelEcsBase* model, ComponentSword* sword, int target_id)
    {
        BuilderDamage().build(model, sword->id, -1, target_id);
        sword->clean = true;
    }

    void SystemSwords::retain()
    {
        ++this->_reference_counter;
    }

    int SystemSwords::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemSwords::operator ==(const SystemSwords& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemSwords::operator !=(const SystemSwords& rhs) const
    {
        return !(*this == rhs);
    }

    SystemSwords::SystemSwords(const SystemSwords& rhs)
    {
        this->operator=(rhs);
    }

    const SystemSwords& SystemSwords::operator =(const SystemSwords& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemSwords::get_type() const
    {
        return SystemSwords::TYPE;
    }

    void SystemSwords::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemSwords::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemSwords::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemSwords::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
