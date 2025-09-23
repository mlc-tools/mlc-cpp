#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ComponentHealth.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "SystemDeath.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemDeath::TYPE("SystemDeath");

    SystemDeath::SystemDeath()
    : _reference_counter(1)
    {

    }

    SystemDeath::~SystemDeath()
    {
    }

    void SystemDeath::update(ModelEcsBase* model, float dt)
    {
        for(auto& health : model->components_health)
        {
            if(health->get_current() <= 0 && !is_ressurection(model, health))
            {
                this->death(model, health);
            }
        }
    }

    void SystemDeath::clean(ModelEcsBase* model)
    {
        for(int __index__ = 0; __index__ < model->components_health.size(); ++__index__)
        {
            auto& component = model->components_health.at(__index__);
            if(component->get_current() <= 0 )
            {
                auto __size__ = model->components_health.size();
                model->remove_entity(component->id);
                if(__size__ != model->components_health.size()) { --__index__; }
            }
        }
    }

    void SystemDeath::death(ModelEcsBase* model, ComponentHealth* health)
    {
        model->event_death[health->id].notify();
    }

    bool SystemDeath::is_ressurection(ModelEcsBase* model, ComponentHealth* health)
    {
        auto stats = model->get<ComponentStats>(health->id);
        auto ressurection = stats->get(model, UnitStat::ressurection);
        if(health->ressurection_count < ressurection)
        {
            health->ressurection_count += 1;
            health->set_current(health->max);

            model->event_ressurection[health->id].notify();
            return true;
        }
        return false;
    }

    void SystemDeath::retain()
    {
        ++this->_reference_counter;
    }

    int SystemDeath::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemDeath::operator ==(const SystemDeath& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemDeath::operator !=(const SystemDeath& rhs) const
    {
        return !(*this == rhs);
    }

    SystemDeath::SystemDeath(const SystemDeath& rhs)
    {
        this->operator=(rhs);
    }

    const SystemDeath& SystemDeath::operator =(const SystemDeath& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemDeath::get_type() const
    {
        return SystemDeath::TYPE;
    }

    void SystemDeath::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemDeath::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemDeath::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemDeath::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
