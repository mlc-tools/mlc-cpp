#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ComponentBusy.h"
#include "ComponentMeleeAttack.h"
#include "ComponentRecharge.h"
#include "ComponentStats.h"
#include "ComponentStun.h"
#include "ModelEcsBase.h"
#include "SystemMeleeAttack.h"
#include "Transform.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemMeleeAttack::TYPE("SystemMeleeAttack");

    SystemMeleeAttack::SystemMeleeAttack()
    : _reference_counter(1)
    {

    }

    SystemMeleeAttack::~SystemMeleeAttack()
    {
    }

    void SystemMeleeAttack::update(ModelEcsBase* model, float dt)
    {
        model->each_if<ComponentMeleeAttack, ComponentRecharge, ComponentBusy, Transform>(
        [&](auto& component_attack, auto& component_recharge, auto& busy, auto& transform)
        {
            auto stun = model->get<ComponentStun>(component_recharge->id);
            bool stunned = stun && stun->activated;
            return !busy->is_busy && !stunned && component_recharge->recharge <= 0;
        },
        [&](auto& component_attack, auto& component_recharge, auto& busy, auto& transform)
        {
            this->try_attack(model, component_attack, component_recharge, transform);
        });
    }

    void SystemMeleeAttack::try_attack(ModelEcsBase* model, ComponentMeleeAttack* component, ComponentRecharge* component_recharge, Transform* transform)
    {
        auto transform_hero = model->get<Transform>(model->player_id);
        float size = 20;
        size += model->get<ComponentStats>(component->id)->get(model, UnitStat::size);
        size += model->get<ComponentStats>(model->player_id)->get(model, UnitStat::size);
        if(transform_hero->position.get_distance_sq(transform->position) <= size*size)
        {
            this->damage(model, component, component_recharge, model->player_id);
        }
    }

    void SystemMeleeAttack::damage(ModelEcsBase* model, ComponentMeleeAttack* component, ComponentRecharge* component_recharge, int target)
    {
        model->event_prepare_to_shoot[component->id].notify(target);
        model->event_shoot[component->id].notify(target);
        component_recharge->recharge = model->get<ComponentStats>(component->id)->get(model, UnitStat::recharge);
    }

    void SystemMeleeAttack::retain()
    {
        ++this->_reference_counter;
    }

    int SystemMeleeAttack::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemMeleeAttack::operator ==(const SystemMeleeAttack& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemMeleeAttack::operator !=(const SystemMeleeAttack& rhs) const
    {
        return !(*this == rhs);
    }

    SystemMeleeAttack::SystemMeleeAttack(const SystemMeleeAttack& rhs)
    {
        this->operator=(rhs);
    }

    const SystemMeleeAttack& SystemMeleeAttack::operator =(const SystemMeleeAttack& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemMeleeAttack::get_type() const
    {
        return SystemMeleeAttack::TYPE;
    }

    void SystemMeleeAttack::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemMeleeAttack::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemMeleeAttack::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemMeleeAttack::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
