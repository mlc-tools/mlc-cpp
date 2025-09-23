#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "BuilderEntity.h"
#include "ComponentHealth.h"
#include "ComponentSide.h"
#include "ComponentStats.h"
#include "ComponentSword.h"
#include "ComponentSwordCast.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include "SystemSwordsCast.h"
#include "Transform.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemSwordsCast::TYPE("SystemSwordsCast");

    SystemSwordsCast::SystemSwordsCast()
    : _reference_counter(1)
    {

    }

    SystemSwordsCast::~SystemSwordsCast()
    {
    }

    void SystemSwordsCast::update(ModelEcsBase* model, float dt)
    {
        if(model->wave_finished)
        {
            return;
        }
        model->each<ComponentSwordCast, Transform, ComponentStats, ComponentSide>(
        [&](auto& sword_cast, auto& transform, auto& stats, auto& side)
        {
            auto count_time_1 = stats->get(model, UnitStat::sword_time_1);
            auto count_time_10 = stats->get(model, UnitStat::sword_time_10);
            auto sword_count_mul = stats->get(model, UnitStat::sword_count_mul);
            if(count_time_1 > 0)
            {
                sword_cast->timer_1 -= dt;
                if(sword_cast->timer_1 <= 0)
                {
                    this->cast_sword(model, sword_cast, transform, stats, side, count_time_1 * sword_count_mul);
                    sword_cast->timer_1 = 1;
                }
            }
            if(count_time_10 > 0)
            {
                sword_cast->timer_10 -= dt;
                if(sword_cast->timer_10 <= 0)
                {
                    this->cast_sword(model, sword_cast, transform, stats, side, count_time_10 * sword_count_mul);
                    sword_cast->timer_10 = 10;
                }
            }
        });
    }

    void SystemSwordsCast::clean(ModelEcsBase* model)
    {
        for(int __index__ = 0; __index__ < model->components_sword.size(); ++__index__)
        {
            auto& sword = model->components_sword.at(__index__);
            if(sword->clean )
            {
                auto __size__ = model->components_sword.size();
                this->remove_sword(model, sword);
                if(__size__ != model->components_sword.size()) { --__index__; }
            }
        }
    }

    void SystemSwordsCast::remove_sword(ModelEcsBase* model, ComponentSword* sword)
    {
        auto health = model->get<ComponentHealth>(sword->target_id);
        if(health)
        {
            health->remove_future_damage(sword->id);
        }
        model->remove_entity(sword->id);
    }

    void SystemSwordsCast::cast_sword(ModelEcsBase* model, ComponentSwordCast* sword_cast, Transform* transform, ComponentStats* stats, ComponentSide* side, int count)
    {
        auto sword_damage = stats->get(model, UnitStat::sword_damage);
        for(int i=0; i<count; ++i)
        {
            auto data = DataStorage::shared().get<DataUnit>("entity_sword");
            auto id = BuilderEntity(data).set_side(side->side).set_position(transform->position).build(model);
            auto sword = model->get<ComponentSword>(id);
            auto sword_stats = model->get<ComponentStats>(id);

            sword->appearance_duration = sword->appearance_duration + random_float() * 0.1f;
            sword->prepare_to_fly_duration = sword->prepare_to_fly_duration + random_float() * 0.2f;
            sword->fly_speed = sword->fly_speed + random_float() * 100;

            sword->timer = sword->appearance_duration;
            sword_stats->set_fixed(UnitStat::damage, stats->get(model, UnitStat::damage) * sword_damage);
            model->event_create_bullet.notify(id, transform->position);
        }
    }

    void SystemSwordsCast::retain()
    {
        ++this->_reference_counter;
    }

    int SystemSwordsCast::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemSwordsCast::operator ==(const SystemSwordsCast& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemSwordsCast::operator !=(const SystemSwordsCast& rhs) const
    {
        return !(*this == rhs);
    }

    SystemSwordsCast::SystemSwordsCast(const SystemSwordsCast& rhs)
    {
        this->operator=(rhs);
    }

    const SystemSwordsCast& SystemSwordsCast::operator =(const SystemSwordsCast& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemSwordsCast::get_type() const
    {
        return SystemSwordsCast::TYPE;
    }

    void SystemSwordsCast::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemSwordsCast::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemSwordsCast::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemSwordsCast::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
