#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BuilderEntity.h"
#include "ComponentCreateUnitsOnDeath.h"
#include "ComponentHealth.h"
#include "ComponentSide.h"
#include "ModelEcsBase.h"
#include "SystemCreateUnitsOnDeath.h"
#include "Transform.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemCreateUnitsOnDeath::TYPE("SystemCreateUnitsOnDeath");

    SystemCreateUnitsOnDeath::SystemCreateUnitsOnDeath()
    : _reference_counter(1)
    {

    }

    SystemCreateUnitsOnDeath::~SystemCreateUnitsOnDeath()
    {
    }

    void SystemCreateUnitsOnDeath::update(ModelEcsBase* model, float dt)
    {
        model->each_if<ComponentCreateUnitsOnDeath, ComponentHealth>(
        [&](auto& component, auto& health)
        {
            return health->get_current() <= 0;
        },
        [&](auto& component, auto& health)
        {
            this->create_units(model, component);
        });
    }

    void SystemCreateUnitsOnDeath::create_units(ModelEcsBase* model, ComponentCreateUnitsOnDeath* component)
    {
        int count = static_cast<int>(component->units.size());
        auto points = Vector::get_points_on_radius(component->radius, count);
        auto transform = model->get<Transform>(component->id);
        for(int i=0; i<count; ++i)
        {
            auto pos = transform->position + points[i];
            if(model->ground->is_bound(pos) && !model->ground->has_collision_with_walls(pos, 0))
            {

                auto data = component->units[i];
                auto level = 1;
                auto side = model->get<ComponentSide>(component->id)->side;
                auto id = BuilderEntity(data).set_level(level).set_side(side).set_position(pos).build(model);
                model->event_create_entity.notify(id, pos);
            }
        }
    }

    void SystemCreateUnitsOnDeath::retain()
    {
        ++this->_reference_counter;
    }

    int SystemCreateUnitsOnDeath::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemCreateUnitsOnDeath::operator ==(const SystemCreateUnitsOnDeath& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemCreateUnitsOnDeath::operator !=(const SystemCreateUnitsOnDeath& rhs) const
    {
        return !(*this == rhs);
    }

    SystemCreateUnitsOnDeath::SystemCreateUnitsOnDeath(const SystemCreateUnitsOnDeath& rhs)
    {
        this->operator=(rhs);
    }

    const SystemCreateUnitsOnDeath& SystemCreateUnitsOnDeath::operator =(const SystemCreateUnitsOnDeath& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemCreateUnitsOnDeath::get_type() const
    {
        return SystemCreateUnitsOnDeath::TYPE;
    }

    void SystemCreateUnitsOnDeath::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemCreateUnitsOnDeath::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemCreateUnitsOnDeath::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemCreateUnitsOnDeath::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
