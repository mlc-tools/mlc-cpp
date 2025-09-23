#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ComponentAura.h"
#include "ComponentAuraOfSlow.h"
#include "ComponentMovement.h"
#include "ModelEcsBase.h"
#include "SystemAuroOfSlow.h"
#include "Transform.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemAuroOfSlow::TYPE("SystemAuroOfSlow");

    SystemAuroOfSlow::SystemAuroOfSlow()
    : _reference_counter(1)
    {

    }

    SystemAuroOfSlow::~SystemAuroOfSlow()
    {
    }

    void SystemAuroOfSlow::update(ModelEcsBase* model, float dt)
    {
        model->each<ComponentAuraOfSlow>(
        [&](auto& aura)
        {
            if(!aura->initialized)
            {
                aura->initialized = true;
                model->event_aura_activated[aura->id].notify(aura);
            }
            float radius_sq = aura->radius * aura->radius;
            auto aura_transform = model->get<Transform>(aura->id);

            model->each<ComponentMovement, Transform>(
            [&](auto& movement, auto& transform)
            {
                if(movement->id == aura->id)
                {
                    return;
                }
                auto offset = transform->position - aura_transform->position;
                offset.y /= 0.8f;
                auto dist_sq = offset.length_sq();
                movement->aura_slow = dist_sq <= radius_sq ? aura : nullptr;
                movement->slow_k = dist_sq <= radius_sq ? std::max(0.5f, dist_sq / radius_sq) : 1;
            });
        });
    }

    void SystemAuroOfSlow::retain()
    {
        ++this->_reference_counter;
    }

    int SystemAuroOfSlow::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemAuroOfSlow::operator ==(const SystemAuroOfSlow& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemAuroOfSlow::operator !=(const SystemAuroOfSlow& rhs) const
    {
        return !(*this == rhs);
    }

    SystemAuroOfSlow::SystemAuroOfSlow(const SystemAuroOfSlow& rhs)
    {
        this->operator=(rhs);
    }

    const SystemAuroOfSlow& SystemAuroOfSlow::operator =(const SystemAuroOfSlow& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemAuroOfSlow::get_type() const
    {
        return SystemAuroOfSlow::TYPE;
    }

    void SystemAuroOfSlow::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemAuroOfSlow::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemAuroOfSlow::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemAuroOfSlow::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
