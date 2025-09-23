#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ComponentHealingLowHp.h"
#include "ComponentHealingOnChangeMaxHp.h"
#include "ComponentHealth.h"
#include "ComponentRandomHealing.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "SystemHealingOnChangeMaxHp.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemHealingOnChangeMaxHp::TYPE("SystemHealingOnChangeMaxHp");

    SystemHealingOnChangeMaxHp::SystemHealingOnChangeMaxHp()
    : _reference_counter(1)
    {

    }

    SystemHealingOnChangeMaxHp::~SystemHealingOnChangeMaxHp()
    {
    }

    void SystemHealingOnChangeMaxHp::update(ModelEcsBase* model, float dt)
    {
        model->each_if<ComponentHealingOnChangeMaxHp,ComponentHealth, ComponentStats>(
        [&](auto& component_healing, auto& component_health, auto& stats)
        {
            component_health->max = stats->get(model, UnitStat::health);
            return component_healing->current_max_hp != static_cast<int>(component_health->max);
        },
        [&](auto& component_healing, auto& component_health, auto& stats)
        {
            if(component_healing->current_max_hp == 0 || component_healing->current_max_hp > component_health->max)
            {
                component_healing->current_max_hp = component_health->max;
                assert(component_healing->current_max_hp != 0);
            }

            float percent = component_health->get_current() / component_healing->current_max_hp;
            auto current = component_health->max * percent;
            auto add = current - component_health->get_current();
            if(add > 0)
            {
                component_health->set_current(component_health->max * percent);
                component_healing->current_max_hp = component_health->max;

                model->event_healing[component_healing->id].notify(add);
            }

        });


        if(!model->wave_finished)
        {
            model->each_if<ComponentRandomHealing,ComponentHealth>(
            [&](auto& random_healing, auto& health)
            {
                random_healing->timer -= dt;
                return random_healing->timer <= 0 && health->get_current() < health->max;
            },
            [&](auto& random_healing, auto& health)
            {
                random_healing->timer = 1;
                if(random_float() < random_healing->chance)
                {
                    random_healing->chance = 0.f;
                    auto value = random_float() * 0.05f + 0.01f;
                    value *= health->max;
                    value = std::min(health->max - health->get_current(), value);
                    health->set_current(health->get_current() + value);

                    model->event_healing[health->id].notify(value);
                }
                else
                {
                    random_healing->chance += 0.005f;
                    random_healing->chance = std::min(0.5f, random_healing->chance);
                }
            });

            model->each_if<ComponentHealingLowHp,ComponentHealth>(
            [&](auto& healing, auto& health)
            {
                return health->get_current() < health->max * 0.3f && health->get_current() > 0;
            },
            [&](auto& healing, auto& health)
            {
                auto limit = health->max * 0.3f;
                auto add = healing->speed * health->max * dt;
                add = std::min(limit - health->get_current(), add);
                if(add > 0)
                {
                    health->set_current(health->get_current() + add);

                    model->event_healing[health->id].notify(add);
                }
            });
        }
    }

    void SystemHealingOnChangeMaxHp::retain()
    {
        ++this->_reference_counter;
    }

    int SystemHealingOnChangeMaxHp::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemHealingOnChangeMaxHp::operator ==(const SystemHealingOnChangeMaxHp& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemHealingOnChangeMaxHp::operator !=(const SystemHealingOnChangeMaxHp& rhs) const
    {
        return !(*this == rhs);
    }

    SystemHealingOnChangeMaxHp::SystemHealingOnChangeMaxHp(const SystemHealingOnChangeMaxHp& rhs)
    {
        this->operator=(rhs);
    }

    const SystemHealingOnChangeMaxHp& SystemHealingOnChangeMaxHp::operator =(const SystemHealingOnChangeMaxHp& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemHealingOnChangeMaxHp::get_type() const
    {
        return SystemHealingOnChangeMaxHp::TYPE;
    }

    void SystemHealingOnChangeMaxHp::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemHealingOnChangeMaxHp::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemHealingOnChangeMaxHp::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemHealingOnChangeMaxHp::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
