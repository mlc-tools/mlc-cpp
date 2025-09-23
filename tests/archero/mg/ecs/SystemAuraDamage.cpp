#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderDamage.h"
#include "ComponentAura.h"
#include "ComponentAuraDamage.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "SystemAuraDamage.h"
#include "Transform.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemAuraDamage::TYPE("SystemAuraDamage");

    SystemAuraDamage::SystemAuraDamage()
    : _reference_counter(1)
    {

    }

    SystemAuraDamage::~SystemAuraDamage()
    {
    }

    void SystemAuraDamage::update(ModelEcsBase* model, float dt)
    {
        model->each<ComponentAuraDamage>(
        [&](auto& aura)
        {
            if(!aura->initialized)
            {
                aura->initialized = true;
                model->event_aura_activated[aura->id].notify(aura);
            }
            float radius_sq = aura->radius * aura->radius;
            auto aura_transform = model->get<Transform>(aura->id);
            auto aura_stats = model->get<ComponentStats>(aura->id);

            model->each<Transform>(
            [&](auto& transform)
            {
                if(transform->id == aura->id)
                {
                    return;
                }
                if(aura->damaged_entities[transform->id] > 0)
                {
                    aura->damaged_entities[transform->id] -= dt;
                    return;
                }
                auto dist_sq = transform->position.get_distance_sq(aura_transform->position);
                if(dist_sq < radius_sq)
                {
                    aura->damaged_entities[transform->id] = 2;

                    float damage = aura_stats->get(model, UnitStat::damage) * aura->damage_mul;
                    BuilderDamage().set_damage(damage).build(model, aura->id, -1, transform->id);
                }
            });
        });
    }

    void SystemAuraDamage::retain()
    {
        ++this->_reference_counter;
    }

    int SystemAuraDamage::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemAuraDamage::operator ==(const SystemAuraDamage& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemAuraDamage::operator !=(const SystemAuraDamage& rhs) const
    {
        return !(*this == rhs);
    }

    SystemAuraDamage::SystemAuraDamage(const SystemAuraDamage& rhs)
    {
        this->operator=(rhs);
    }

    const SystemAuraDamage& SystemAuraDamage::operator =(const SystemAuraDamage& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemAuraDamage::get_type() const
    {
        return SystemAuraDamage::TYPE;
    }

    void SystemAuraDamage::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemAuraDamage::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemAuraDamage::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemAuraDamage::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
