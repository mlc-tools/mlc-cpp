#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentEffects.h"
#include "ModelEcsBase.h"
#include "SystemEffects.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemEffects::TYPE("SystemEffects");

    SystemEffects::SystemEffects()
    : _reference_counter(1)
    {

    }

    SystemEffects::~SystemEffects()
    {
    }

    void SystemEffects::update(ModelEcsBase* model, float dt)
    {
        for(auto& component : model->components_effects)
        {
            if(component->damage_mul_timer > 0)
            {
                component->damage_mul_timer -= dt;
                if(component->damage_mul_timer <= 0)
                {

                    model->event_effects[component->id].notify("deactivate_timed_damage");
                }
            }
        }
    }

    void SystemEffects::retain()
    {
        ++this->_reference_counter;
    }

    int SystemEffects::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemEffects::operator ==(const SystemEffects& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemEffects::operator !=(const SystemEffects& rhs) const
    {
        return !(*this == rhs);
    }

    SystemEffects::SystemEffects(const SystemEffects& rhs)
    {
        this->operator=(rhs);
    }

    const SystemEffects& SystemEffects::operator =(const SystemEffects& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemEffects::get_type() const
    {
        return SystemEffects::TYPE;
    }

    void SystemEffects::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemEffects::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemEffects::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemEffects::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
