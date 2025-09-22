#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BuilderBullet.h"
#include "ComponentHealth.h"
#include "ComponentMeteorPeriodic.h"
#include "ComponentSide.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "SystemMeteor.h"
#include "Transform.h"
#include "UnitStat.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemMeteor::TYPE("SystemMeteor");

    SystemMeteor::SystemMeteor()
    : _reference_counter(1)
    {

    }

    SystemMeteor::~SystemMeteor()
    {
    }

    void SystemMeteor::update(ModelEcsBase* model, float dt)
    {
        this->dispatch_periodic_meteor(model, dt);
    }

    void SystemMeteor::dispatch_periodic_meteor(ModelEcsBase* model, float dt)
    {
        model->each<ComponentMeteorPeriodic, Transform>(
        [&](auto& component, auto& transform)
        {
            component->timer -= dt;
            if(component->timer <= 0)
            {
                component->timer = component->period;
                auto target_id = SystemMeteor::get_random_enemy(model, component->id);
                if(target_id > 0)
                {
                    auto stats = model->get<ComponentStats>(component->id);
                    int count = 1 + stats->get(model, UnitStat::meteor_count);
                    SystemMeteor::create_meteor(model, component->id, count, target_id);
                }
            }
        });
    }

    void SystemMeteor::create_meteor(ModelEcsBase* model, int shooter_id, int count, int target_id)
    {
        for(int i = 0; i < count; ++i)
        {
            int current_target = target_id;
            if(current_target <= 0 || i > 0)
            {
                current_target = SystemMeteor::get_random_enemy(model, shooter_id);
            }
            if(current_target <= 0)
            {
                continue;
            }
            auto target_transform = model->get<Transform>(current_target);
            auto start_pos = target_transform->position;
            BuilderBullet(shooter_id)
            .set_name("meteor")
            .set_target_id(current_target)
            .set_position_create(start_pos)
            .set_position_to_shoot(target_transform->position)
            .build(model);
        }
    }

    int SystemMeteor::get_random_enemy(ModelEcsBase* model, int shooter_id)
    {
        auto side_shooter = model->get<ComponentSide>(shooter_id);
        std::vector<int> enemies;
        model->each<ComponentSide, ComponentHealth>(
        [&](auto& side, auto& health)
        {
            if(side_shooter->is_enemy(side->side) && health->get_current() > 0)
            {
                enemies.push_back(side->id);
            }
        });
        if(enemies.empty())
        {
            return 0;
        }
        auto index = random_int(0, enemies.size());
        return enemies[index];
    }

    void SystemMeteor::retain()
    {
        ++this->_reference_counter;
    }

    int SystemMeteor::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemMeteor::operator ==(const SystemMeteor& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemMeteor::operator !=(const SystemMeteor& rhs) const
    {
        return !(*this == rhs);
    }

    SystemMeteor::SystemMeteor(const SystemMeteor& rhs)
    {
        this->operator=(rhs);
    }

    const SystemMeteor& SystemMeteor::operator =(const SystemMeteor& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemMeteor::get_type() const
    {
        return SystemMeteor::TYPE;
    }

    void SystemMeteor::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemMeteor::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemMeteor::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemMeteor::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
