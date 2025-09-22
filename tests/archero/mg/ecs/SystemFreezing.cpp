#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentStats.h"
#include "Freezing.h"
#include "ModelEcsBase.h"
#include "SystemFreezing.h"
#include "UnitStat.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemFreezing::TYPE("SystemFreezing");

    SystemFreezing::SystemFreezing()
    : _reference_counter(1)
    {

    }

    SystemFreezing::~SystemFreezing()
    {
    }

    void SystemFreezing::add_damage(ModelEcsBase* model, int target_id, ComponentStats* target_stats, float damage)
    {
        auto freeze = model->get<Freezing>(target_id);
        if(!freeze)
        {
            freeze = make_intrusive<Freezing>();
            model->add(freeze, target_id);
        }
        if(freeze->activated || freeze->life_time > 0)
        {
            return;
        }
        if(target_stats)
        {
            auto resist = target_stats->get(model, UnitStat::freezing_resist);
            damage *= (1 - resist);
        }
        freeze->damage += damage;
        if(freeze->damage >= 1 && target_stats)
        {
            freeze->activated = true;
            freeze->duration = 2;
            auto freeze_immunity_time = target_stats->get(model, UnitStat::freeze_immunity_time);
            freeze->life_time = freeze->duration + freeze_immunity_time;
            target_stats->set_fixed(UnitStat::speed, 0.01);
            target_stats->set_fixed(UnitStat::recharge, 9999);
            model->event_freeze_activated[target_id].notify();
        }
    }

    void SystemFreezing::update(ModelEcsBase* model, float dt)
    {
        model->each<Freezing, ComponentStats>(
        [&](auto& freeze, auto& stats)
        {
            if(freeze->activated)
            {
                freeze->duration -= dt;
                freeze->life_time -= dt;
                if(freeze->duration <= 0)
                {
                    freeze->activated = false;
                    stats->reset_cashe(UnitStat::speed);
                    stats->reset_cashe(UnitStat::recharge);
                    freeze->damage = 0;
                    model->event_freeze_deactivated[freeze->id].notify();
                }
            }
            else if(freeze->life_time > 0)
            {
                freeze->life_time -= dt;
                if(freeze->life_time < 0)
                {
                    freeze->life_time = 0;
                }
            }
        });
    }

    void SystemFreezing::retain()
    {
        ++this->_reference_counter;
    }

    int SystemFreezing::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemFreezing::operator ==(const SystemFreezing& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemFreezing::operator !=(const SystemFreezing& rhs) const
    {
        return !(*this == rhs);
    }

    SystemFreezing::SystemFreezing(const SystemFreezing& rhs)
    {
        this->operator=(rhs);
    }

    const SystemFreezing& SystemFreezing::operator =(const SystemFreezing& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemFreezing::get_type() const
    {
        return SystemFreezing::TYPE;
    }

    void SystemFreezing::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemFreezing::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemFreezing::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemFreezing::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
