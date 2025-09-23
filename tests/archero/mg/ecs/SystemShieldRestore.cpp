#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ComponentShield.h"
#include "ComponentShieldType.h"
#include "ModelEcsBase.h"
#include "SystemShieldRestore.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemShieldRestore::TYPE("SystemShieldRestore");

    SystemShieldRestore::SystemShieldRestore()
    : _reference_counter(1)
    {

    }

    SystemShieldRestore::~SystemShieldRestore()
    {
    }

    void SystemShieldRestore::update(ModelEcsBase* model, float dt)
    {
        for(auto& component : model->components_shield)
        {
            if(!component->initialized && component->activate_type == ComponentShieldType::wave)
            {
                model->event_wave_start.add(component, [component, model]() mutable
                {
                    component->active = true;
                    model->event_shield_activated[component->id].notify();
                });
                component->initialized = true;
            }
            if(component->activate_type == ComponentShieldType::timer && !component->active)
            {
                component->timer -= dt;
                if(component->timer <= 0)
                {
                    component->active = true;
                    model->event_shield_activated[component->id].notify();
                    component->timer = 30;
                }
            }
        }
    }

    void SystemShieldRestore::retain()
    {
        ++this->_reference_counter;
    }

    int SystemShieldRestore::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemShieldRestore::operator ==(const SystemShieldRestore& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemShieldRestore::operator !=(const SystemShieldRestore& rhs) const
    {
        return !(*this == rhs);
    }

    SystemShieldRestore::SystemShieldRestore(const SystemShieldRestore& rhs)
    {
        this->operator=(rhs);
    }

    const SystemShieldRestore& SystemShieldRestore::operator =(const SystemShieldRestore& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemShieldRestore::get_type() const
    {
        return SystemShieldRestore::TYPE;
    }

    void SystemShieldRestore::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemShieldRestore::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemShieldRestore::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemShieldRestore::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
