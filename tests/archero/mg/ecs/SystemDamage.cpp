#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentDamage.h"
#include "ComponentData.h"
#include "ComponentEffects.h"
#include "ComponentHealth.h"
#include "ComponentShield.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "SystemDamage.h"
#include "SystemElectricDamage.h"
#include "SystemFireDamage.h"
#include "SystemFreezing.h"
#include "SystemStun.h"
#include "SystemVampire.h"
#include "Transform.h"
#include "UnitStat.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemDamage::TYPE("SystemDamage");

    SystemDamage::SystemDamage()
    : _reference_counter(1)
    {

    }

    SystemDamage::~SystemDamage()
    {
    }

    void SystemDamage::update(ModelEcsBase* model, float dt)
    {
        for(auto& component : model->components_damage)
        {
            auto shield = model->get<ComponentShield>(component->target_id);
            if(shield && shield->active)
            {
                shield->active = false;
                continue;
            }
            auto health = model->get<ComponentHealth>(component->target_id);
            auto stats_attacker = model->get<ComponentStats>(component->attacker_id);
            auto stats_target = model->get<ComponentStats>(component->target_id);
            auto damage = component->damage;
            if(damage == 0)
            {
                damage = SystemDamage::compute_damage(model, component->attacker_id, component->target_id, stats_attacker);
            }
            auto is_dodge = SystemDamage::is_dodge(model, component->attacker_id, component->target_id, stats_attacker, stats_target);
            auto freeze_chance = SystemDamage::compute_freeze_chance(model, component, component->attacker_id, component->target_id, stats_attacker, stats_target);
            auto freeze_damage = SystemDamage::compute_freeze_damage(model, component, component->attacker_id, component->target_id, stats_attacker, stats_target);
            auto stun_chance = SystemDamage::compute_stun_chance(model, component, component->attacker_id, component->target_id, stats_attacker, stats_target);
            auto stun_duration = SystemDamage::compute_stun_duration(model, component, component->attacker_id, component->target_id, stats_attacker, stats_target);
            auto fire_damage = SystemDamage::compute_fire_damage(model, component, component->attacker_id, component->target_id, stats_attacker, stats_target);
            auto electric_damage = SystemDamage::compute_electric_damage(model, component, component->attacker_id, component->target_id, stats_attacker, stats_target);

            if(is_dodge)
            {

                model->event_dodge[component->target_id].notify();
                if(health)
                {
                    health->remove_future_damage(component->attacker_id);
                }
            }
            else
            {
                if(damage == 0)
                {
                    damage = component->damage;
                }
                if(health && damage > 0)
                {
                    health->damage(component->attacker_id, damage);
                    model->event_damaged[health->id].notify(damage);
                    this->check_effects_after_damage(model, component, stats_attacker, stats_target);

                    if(freeze_damage > 0)
                    {
                        SystemFreezing::add_damage(model, component->target_id, stats_target, freeze_damage);
                    }
                    if(freeze_chance > 0 && random_float() < freeze_chance)
                    {
                        SystemFreezing::add_damage(model, component->target_id, stats_target, 1);
                    }
                    if(stun_chance > 0 && random_float() < stun_chance)
                    {
                        SystemStun::apply(model, component->target_id, std::max(0.5f, stun_duration));
                    }
                    if(fire_damage > 0)
                    {
                        SystemFireDamage::add_damage(model, component->attacker_id, component->target_id, fire_damage);
                    }
                    if(electric_damage > 0)
                    {
                        SystemElectricDamage::add_damage(model, component->attacker_id, component->target_id, electric_damage);
                    }
                    if(health->get_current() <= 0)
                    {
                        SystemVampire::on_kill(model, component->attacker_id, component->target_id);
                        this->check_effects_on_kill(model, stats_attacker, stats_target);
                    }
                }
            }
        }
    }

    float SystemDamage::compute_damage(ModelEcsBase* model, int shooter_id, int target_id, ComponentStats* stats_attacker)
    {
        if(!stats_attacker)
        {
            stats_attacker = model->get<ComponentStats>(shooter_id);
        }
        float damage = 0;
        if(stats_attacker)
        {
            damage = stats_attacker->get(model, UnitStat::damage);
            auto damage_distance = stats_attacker->get(model, UnitStat::damage_distance);
            if(damage_distance > 0)
            {
                auto transform_shooter = model->get<Transform>(shooter_id);
                auto transform_target = model->get<Transform>(target_id);
                if(transform_shooter && transform_target)
                {
                    auto dist = transform_shooter->position.get_distance(transform_target->position);
                    damage *= 1.f + dist / 1000.f * damage_distance;
                }
            }

            if(auto target_data = model->get<ComponentData>(target_id))
            {
                if(target_data->data && target_data->data->is_boss)
                {
                    auto bonus = stats_attacker->get(model, UnitStat::damage_to_boss);
                    if(bonus > 0)
                    {
                        damage *= 1.f + bonus;
                    }
                }
            }
        }
        else if(auto health = model->get<ComponentHealth>(target_id))
        {
            damage = health->get_future_damage(shooter_id);
        }
        if(auto component_effects = model->get<ComponentEffects>(shooter_id))
        {
            damage = component_effects->modify_damage(damage);
        }
        return damage;
    }

    float SystemDamage::is_dodge(ModelEcsBase* model, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        auto dodge = stats_target->get(model, UnitStat::dodge);
        if(dodge > 0)
        {
            return random_float() < dodge;
        }
        return false;
    }

    float SystemDamage::compute_freeze_chance(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        float chance = 0;
        if(stats_attacker)
        {
            chance += stats_attacker->get(model, UnitStat::freeze_chance);
        }
        if(auto stats = model->get<ComponentStats>(component_damage->bullet_id))
        {
            chance += stats->get(model, UnitStat::freeze_chance);
        }
        return chance;
    }

    float SystemDamage::compute_stun_chance(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        float chance = 0;
        if(stats_attacker)
        {
            chance += stats_attacker->get(model, UnitStat::stun_chance);
        }
        if(auto stats = model->get<ComponentStats>(component_damage->bullet_id))
        {
            chance += stats->get(model, UnitStat::stun_chance);
        }
        return chance;
    }

    float SystemDamage::compute_stun_duration(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        float duration = 0;
        if(stats_attacker)
        {
            duration += stats_attacker->get(model, UnitStat::stun_duration);
        }
        if(auto stats = model->get<ComponentStats>(component_damage->bullet_id))
        {
            duration += stats->get(model, UnitStat::stun_duration);
        }
        return duration;
    }

    float SystemDamage::compute_freeze_damage(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        float freeze_damage = 0;
        if(stats_attacker)
        {
            freeze_damage += stats_attacker->get(model, UnitStat::freeze);
        }
        if(auto stats = model->get<ComponentStats>(component_damage->bullet_id))
        {
            freeze_damage += stats->get(model, UnitStat::freeze);
        }
        return freeze_damage;
    }

    float SystemDamage::compute_fire_damage(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        float fire_damage = 0;
        if(stats_attacker)
        {
            fire_damage += stats_attacker->get(model, UnitStat::fire_damage);
        }
        if(auto stats = model->get<ComponentStats>(component_damage->bullet_id))
        {
            fire_damage += stats->get(model, UnitStat::fire_damage);
        }
        return fire_damage;
    }

    float SystemDamage::compute_electric_damage(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        float electric_damage = 0;
        if(stats_attacker)
        {
            electric_damage += stats_attacker->get(model, UnitStat::electric_damage);
        }
        if(auto stats = model->get<ComponentStats>(component_damage->bullet_id))
        {
            electric_damage += stats->get(model, UnitStat::electric_damage);
        }
        return electric_damage;
    }

    float SystemDamage::compute_future_damage(ModelEcsBase* model, int shooter_id, int bullet_id, int target_id)
    {
        return SystemDamage::compute_damage(model, shooter_id, target_id);
    }

    void SystemDamage::check_effects_after_damage(ModelEcsBase* model, ComponentDamage* damage, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        auto damage_after_damaged = stats_target->get(model, UnitStat::damage_after_damaged);
        if(damage_after_damaged > 0)
        {
            auto component_effects = model->get<ComponentEffects>(damage->target_id);
            if(component_effects)
            {

                model->event_effects[component_effects->id].notify("activate_timed_damage");
                component_effects->damage_mul = damage_after_damaged;
                component_effects->damage_mul_timer = 5;
            }
        }
    }

    void SystemDamage::check_effects_on_kill(ModelEcsBase* model, ComponentStats* stats_attacker, ComponentStats* stats_target)
    {
        auto damage_after_kill = stats_attacker->get(model, UnitStat::damage_after_kill);
        if(damage_after_kill > 0)
        {
            auto component_effects = model->get<ComponentEffects>(stats_attacker->id);
            if(component_effects)
            {

                model->event_effects[component_effects->id].notify("activate_timed_damage");
                component_effects->damage_mul = damage_after_kill;
                component_effects->damage_mul_timer = 5;
            }
        }
    }

    void SystemDamage::retain()
    {
        ++this->_reference_counter;
    }

    int SystemDamage::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemDamage::operator ==(const SystemDamage& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemDamage::operator !=(const SystemDamage& rhs) const
    {
        return !(*this == rhs);
    }

    SystemDamage::SystemDamage(const SystemDamage& rhs)
    {
        this->operator=(rhs);
    }

    const SystemDamage& SystemDamage::operator =(const SystemDamage& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemDamage::get_type() const
    {
        return SystemDamage::TYPE;
    }

    void SystemDamage::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemDamage::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemDamage::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemDamage::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
