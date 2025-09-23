#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderBullet.h"
#include "ComponentHealth.h"
#include "ComponentRecharge.h"
#include "ComponentShootBullet.h"
#include "ComponentSide.h"
#include "ComponentSpineInfo.h"
#include "ComponentStats.h"
#include "ComponentStun.h"
#include "ComponentTargetHighlight.h"
#include "ComponentTargetable.h"
#include "Freezing.h"
#include "ModelEcsBase.h"
#include "MoveDirection.h"
#include "MoveToTarget.h"
#include "Side.h"
#include "SystemMeteor.h"
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
    const std::string SystemShoot::TYPE("SystemShoot");

    SystemShoot::SystemShoot()
    : _reference_counter(1)
    {

    }

    SystemShoot::~SystemShoot()
    {
    }

    void SystemShoot::update(ModelEcsBase* model, float dt)
    {
        for(auto& component : model->components_recharge)
        {
            auto freezing = model->get<Freezing>(component->id);
            if(freezing && freezing->activated)
            {
                continue;
            }
            if(auto stun = model->get<ComponentStun>(component->id))
            {
                if(stun->activated)
                {
                    continue;
                }
            }
            component->recharge -= dt;
            component->attack_animation_timer -= dt;
        }
        model->each_if<ComponentShootBullet, ComponentRecharge>(
        [this, model](auto& component_shoot_bullet, auto& component_recharge)
        {
            return component_recharge->recharge <= 0;
        },
        [this, model](auto& component_shoot_bullet, auto& component_recharge)
        {
            if(component_recharge->recharge <= 0)
            {
                this->try_shoot(model, component_recharge);
            }
        });
    }

    void SystemShoot::try_shoot(ModelEcsBase* model, ComponentRecharge* component)
    {
        assert(component->recharge <= 0);
        auto component_stats = model->get<ComponentStats>(component->id);

        int target = -1;
        auto target_highlight = model->get<ComponentTargetHighlight>(component->id);
        if(target_highlight)
        {
            target = this->find_target(model, component);
            target_highlight->target_id = target;
        }

        bool can_shoot_on_move = component_stats->get(model, UnitStat::attack_on_move) > 0;
        if(!can_shoot_on_move && (model->get<MoveDirection>(component->id) || model->get<MoveToTarget>(component->id)))
        {
            return;
        }

        if(target == -1)
        {
            target = this->find_target(model, component);
        }
        if(target > 0)
        {
            this->shoot(model, component, target);
        }
    }

    int SystemShoot::find_target(ModelEcsBase* model, ComponentRecharge* component)
    {
        auto radius = model->get<ComponentStats>(component->id)->get(model, UnitStat::shoot_radius);
        auto side_shooter = model->get<ComponentSide>(component->id);
        auto transform_shooter = model->get<Transform>(component->id);
        auto target = this->get_nearest_entity_in_radius(model, component->id, radius, side_shooter, transform_shooter);
        return target;
    }

    int SystemShoot::get_nearest_entity_in_radius(ModelEcsBase* model, int shooter, float radius, ComponentSide* side_shooter, Transform* transform_shooter, bool with_future, int prev_target)
    {
        auto min_radius_sq = radius * radius;
        auto min_hp = 10000;
        auto result = 0;

        model->each_if<ComponentSide, ComponentTargetable, ComponentHealth, Transform>(
        [&](auto& component_side, auto& targetable, auto& health, auto& transform)
        {
            return component_side->id != prev_target && side_shooter->is_enemy(component_side->side) && (!with_future || health->get_future_value() > 0);
        },
        [&](auto& component_side, auto& targetable, auto& health, auto& transform)
        {
            auto dist = transform_shooter->position.get_distance_sq(transform->position);
            if(dist <= min_radius_sq || (health->get_future_value() < min_hp && dist <= min_radius_sq * 1.1f))
            {
                result = health->id;
                min_radius_sq = dist;
                min_hp = health->get_future_value();
            }
        });

        if(result == 0 && with_future)
        {
            return get_nearest_entity_in_radius(model, shooter, radius, side_shooter, transform_shooter, false);
        }
        return result;
    }

    void SystemShoot::shoot(ModelEcsBase* model, ComponentRecharge* component, int target)
    {
        auto stats = model->get<ComponentStats>(component->id);
        model->event_prepare_to_shoot[component->id].notify(target);


        float bullets_count = 1 + stats->get(model, UnitStat::bullets_count);
        auto direction = model->get<Transform>(target)->position - model->get<Transform>(component->id)->position;
        auto normal = Vector(0, 0);
        auto offset = Vector(0, 0);
        if(bullets_count > 1)
        {
            normal = Vector::get_normal_to(direction) * 30;
            offset = normal * (bullets_count * 0.5f - 0.5f);
        }
        for(float i=0; i<bullets_count; ++i)
        {
            BuilderBullet(component->id).set_target_id(target).set_offset_create(offset).build(model);

            offset -= normal;
        }


        float bullets_count_back = stats->get(model, UnitStat::bullets_count_back);
        if(bullets_count_back > 0)
        {
            auto back_dir = -direction;
            back_dir.normalize();
            auto back_normal = Vector::get_normal_to(direction) * 30;
            auto back_offset = back_normal * (bullets_count_back * 0.5f - 0.5f);
            for(float i=0; i<bullets_count_back; ++i)
            {
                BuilderBullet(component->id).set_direction(back_dir).set_offset_create(back_offset).build(model);
                back_offset -= back_normal;
            }
        }


        float bullets_count_side = stats->get(model, UnitStat::bullets_count_side);
        if(bullets_count_side > 0)
        {
            auto bullets_count_side_deviation = stats->get(model, UnitStat::bullets_count_side_deviation);
            if(bullets_count_side_deviation == 0)
            {
                bullets_count_side_deviation = 1;
            }
            int angle = 0;
            if(bullets_count_side <= 2)
            {
                angle = 30;
            }
            else if(bullets_count_side <= 4)
            {
                angle = 25;
            }
            else
            {
                angle = 20;
            }
            angle *= bullets_count_side_deviation;
            float radian_angle = angle * M_PI / 180;
            float first = radian_angle * -bullets_count_side / 2;
            for(float i=0; i<bullets_count_side; ++i)
            {
                BuilderBullet(component->id).set_target_id(target).set_deviation_from_direction(first).build(model);

                first += radian_angle;
                if(std::fabs(first) < 0.001f)
                {
                    first += radian_angle;
                }
            }
        }

        auto meteor_chance = stats->get(model, UnitStat::meteor_chance);
        if(meteor_chance > 0 && random_float() < meteor_chance)
        {
            int meteor_count = 1 + stats->get(model, UnitStat::meteor_count);
            SystemMeteor::create_meteor(model, component->id, meteor_count, target);
        }

        model->event_shoot[component->id].notify(target);
        component->recharge = model->get<ComponentStats>(component->id)->get(model, UnitStat::recharge);
        auto __tmp_attack_animation_timer = model->get<ComponentSpineInfo>(component->id);

        if(__tmp_attack_animation_timer)

        {

            component->attack_animation_timer = __tmp_attack_animation_timer->get_animation_duration("attack");

        }
    }

    void SystemShoot::retain()
    {
        ++this->_reference_counter;
    }

    int SystemShoot::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemShoot::operator ==(const SystemShoot& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemShoot::operator !=(const SystemShoot& rhs) const
    {
        return !(*this == rhs);
    }

    SystemShoot::SystemShoot(const SystemShoot& rhs)
    {
        this->operator=(rhs);
    }

    const SystemShoot& SystemShoot::operator =(const SystemShoot& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemShoot::get_type() const
    {
        return SystemShoot::TYPE;
    }

    void SystemShoot::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemShoot::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemShoot::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemShoot::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
