#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBusy.h"
#include "ComponentCreateMovementToHero.h"
#include "ModelEcsBase.h"
#include "MoveToTarget.h"
#include "SystemCreateMovement.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemCreateMovement::TYPE("SystemCreateMovement");

    SystemCreateMovement::SystemCreateMovement()
    : _reference_counter(1)
    {

    }

    SystemCreateMovement::~SystemCreateMovement()
    {
    }

    void SystemCreateMovement::update(ModelEcsBase* model, float dt)
    {
        model->each_if<ComponentCreateMovementToHero, ComponentBusy>(
        [&](auto& component, auto& busy)
        {
            return !busy->is_busy;
        },
        [&](auto& component, auto& busy)
        {
            auto movement = model->get<MoveToTarget>(component->id);
            if(movement)
            {
                return;
            }
            movement = make_intrusive<MoveToTarget>();
            movement->target_id = model->player_id;
            model->add(movement, component->id);
        });
    }

    void SystemCreateMovement::retain()
    {
        ++this->_reference_counter;
    }

    int SystemCreateMovement::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemCreateMovement::operator ==(const SystemCreateMovement& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemCreateMovement::operator !=(const SystemCreateMovement& rhs) const
    {
        return !(*this == rhs);
    }

    SystemCreateMovement::SystemCreateMovement(const SystemCreateMovement& rhs)
    {
        this->operator=(rhs);
    }

    const SystemCreateMovement& SystemCreateMovement::operator =(const SystemCreateMovement& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemCreateMovement::get_type() const
    {
        return SystemCreateMovement::TYPE;
    }

    void SystemCreateMovement::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemCreateMovement::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemCreateMovement::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemCreateMovement::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
