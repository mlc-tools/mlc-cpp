#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BuilderBullet.h"
#include "ComponentHealth.h"
#include "ComponentSide.h"
#include "ComponentStats.h"
#include "ComponentTargetable.h"
#include "ElectricDamage.h"
#include "ModelEcsBase.h"
#include "SystemElectricDamage.h"
#include "Transform.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemElectricDamage::TYPE("SystemElectricDamage");

    SystemElectricDamage::SystemElectricDamage()
    : _reference_counter(1)
    {

    }

    SystemElectricDamage::~SystemElectricDamage()
    {
    }

    void SystemElectricDamage::add_damage(ModelEcsBase* model, int shooter_id, int target_id, float amount)
    {
        auto electric = model->get<ElectricDamage>(target_id);
        if(!electric)
        {
            electric = make_intrusive<ElectricDamage>();
            model->add(electric, target_id);
        }
        electric->value += amount * (1 - model->get<ComponentStats>(target_id)->get(model, UnitStat::electric_resist));
        electric->shooter_id = shooter_id;
        if(electric->value >= 1)
        {
            electric->value = 0;
            auto transform = model->get<Transform>(target_id);
            auto side_shooter = model->get<ComponentSide>(shooter_id);
            int t1 = 0; float d1 = 1000000;
            int t2 = 0; float d2 = 1000000;
            int t3 = 0; float d3 = 1000000;
            if(transform && side_shooter)
            {
                model->each_if<ComponentSide, ComponentTargetable, ComponentHealth, Transform>(
                [&](auto& side, auto& targetable, auto& health, auto& transform_candidate)
                {
                    return side_shooter->is_enemy(side->side) && health->id != target_id && health->get_future_value() > 0;
                },
                [&](auto& side, auto& targetable, auto& health, auto& transform_candidate)
                {
                    auto dist = transform_candidate->position.get_distance_sq(transform->position);
                    if(dist < d1)
                    {
                        t3 = t2; d3 = d2;
                        t2 = t1; d2 = d1;
                        t1 = health->id; d1 = dist;
                    }
                    else if(dist < d2)
                    {
                        t3 = t2; d3 = d2;
                        t2 = health->id; d2 = dist;
                    }
                    else if(dist < d3)
                    {
                        t3 = health->id; d3 = dist;
                    }
                });

                model->event_effects[target_id].notify("electric");
                auto damage =  model->get<ComponentStats>(shooter_id)->get(model, UnitStat::damage) / 6;
                if(t1)
                {
                    BuilderBullet(target_id).set_side(side_shooter->side).set_damage(damage).set_name("spirit_lightning_bullet").set_target_id(t1).build(model);
                }
                if(t2)
                {
                    BuilderBullet(target_id).set_side(side_shooter->side).set_damage(damage).set_name("spirit_lightning_bullet").set_target_id(t2).build(model);
                }
                if(t3)
                {
                    BuilderBullet(target_id).set_side(side_shooter->side).set_damage(damage).set_name("spirit_lightning_bullet").set_target_id(t3).build(model);
                }
            }
        }
    }

    void SystemElectricDamage::retain()
    {
        ++this->_reference_counter;
    }

    int SystemElectricDamage::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemElectricDamage::operator ==(const SystemElectricDamage& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemElectricDamage::operator !=(const SystemElectricDamage& rhs) const
    {
        return !(*this == rhs);
    }

    SystemElectricDamage::SystemElectricDamage(const SystemElectricDamage& rhs)
    {
        this->operator=(rhs);
    }

    const SystemElectricDamage& SystemElectricDamage::operator =(const SystemElectricDamage& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemElectricDamage::get_type() const
    {
        return SystemElectricDamage::TYPE;
    }

    void SystemElectricDamage::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemElectricDamage::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemElectricDamage::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemElectricDamage::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
