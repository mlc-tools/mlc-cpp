#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderDamage.h"
#include "ComponentStats.h"
#include "FireDamage.h"
#include "ModelEcsBase.h"
#include "SystemDamage.h"
#include "SystemFireDamage.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemFireDamage::TYPE("SystemFireDamage");

    SystemFireDamage::SystemFireDamage()
    : _reference_counter(1)
    {

    }

    SystemFireDamage::~SystemFireDamage()
    {
    }

    void SystemFireDamage::add_damage(ModelEcsBase* model, int shooter_id, int target_id, float amount)
    {
        auto fire = model->get<FireDamage>(target_id);
        if(!fire)
        {
            fire = make_intrusive<FireDamage>();
            model->add(fire, target_id);
        }
        if(fire->burning)
        {
            return;
        }
        fire->value += amount * (1 - model->get<ComponentStats>(target_id)->get(model, UnitStat::fire_resist));
        fire->shooter_id = shooter_id;
        if(fire->value >= 1)
        {
            fire->burning = true;
            fire->burning_timer = 5;
            fire->damage_timer = 1;
            model->event_fire_activated[fire->id].notify();
        }
    }

    void SystemFireDamage::update(ModelEcsBase* model, float dt)
    {
        model->each<FireDamage>(
        [&](auto& fire)
        {
            if(fire->burning)
            {
                fire->burning_timer -= dt;
                fire->damage_timer -= dt;
                if(fire->damage_timer <= 0)
                {
                    fire->damage_timer += 1;
                    auto damage = SystemDamage::compute_damage(model, fire->shooter_id, fire->id);
                    BuilderDamage().set_damage(damage * 0.2).build(model, fire->shooter_id, -1, fire->id);
                }
                if(fire->burning_timer <= 0)
                {
                    fire->burning = false;
                    fire->value = 0;
                    fire->shooter_id = 0;
                    fire->damage_timer = 0;
                    model->event_fire_deactivated[fire->id].notify();
                }
            }
        });
    }

    void SystemFireDamage::retain()
    {
        ++this->_reference_counter;
    }

    int SystemFireDamage::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemFireDamage::operator ==(const SystemFireDamage& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemFireDamage::operator !=(const SystemFireDamage& rhs) const
    {
        return !(*this == rhs);
    }

    SystemFireDamage::SystemFireDamage(const SystemFireDamage& rhs)
    {
        this->operator=(rhs);
    }

    const SystemFireDamage& SystemFireDamage::operator =(const SystemFireDamage& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemFireDamage::get_type() const
    {
        return SystemFireDamage::TYPE;
    }

    void SystemFireDamage::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemFireDamage::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemFireDamage::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemFireDamage::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
