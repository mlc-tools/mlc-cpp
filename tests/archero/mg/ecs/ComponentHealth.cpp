#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentHealth.h"
#include "ModelEcsBase.h"
#include <map>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentHealth::TYPE("ComponentHealth");

    ComponentHealth::ComponentHealth()
    : current(0)
    , max(0)
    , future_damage()
    , sum_future_damage(0.0f)
    , ressurection_count(0)
    {

    }

    ComponentHealth::~ComponentHealth()
    {
    }

    float ComponentHealth::get_current() const
    {
        return this->current;
    }

    void ComponentHealth::set_current(float value)
    {
        this->current = value;
    }

    float ComponentHealth::get_future_damage(int damager)
    {
        if(in_map(damager, this->future_damage))
        {
            return this->future_damage.at(damager);
        }
        return 0;
    }

    void ComponentHealth::add_future_damage(int damager, float damage)
    {
        this->future_damage[damager] = damage;
        this->calculate_future_damage();
    }

    void ComponentHealth::remove_future_damage(int damager)
    {
        map_remove(this->future_damage, damager);
        this->calculate_future_damage();
    }

    void ComponentHealth::damage(int damager, float damage)
    {
        map_remove(this->future_damage, damager);
        this->calculate_future_damage();

        this->current -= std::min(this->current, damage);
    }

    void ComponentHealth::calculate_future_damage()
    {
        this->sum_future_damage = 0;
        for (auto&& pair : this->future_damage)
        {
            auto& _ = pair.first;
            auto& value = pair.second;
            (void)_; //don't generate 'Unused variable' warning
            (void)value; //don't generate 'Unused variable' warning
            this->sum_future_damage += value;
        }
    }

    float ComponentHealth::get_future_value() const
    {
        return this->current - this->sum_future_damage;
    }

    bool ComponentHealth::operator ==(const ComponentHealth& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->current == rhs.current;
        result = result && this->max == rhs.max;
        result = result && this->future_damage == rhs.future_damage;
        result = result && this->sum_future_damage == rhs.sum_future_damage;
        result = result && this->ressurection_count == rhs.ressurection_count;
        return result;
    }

    bool ComponentHealth::operator !=(const ComponentHealth& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentHealth::ComponentHealth(const ComponentHealth& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentHealth& ComponentHealth::operator =(const ComponentHealth& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->current = rhs.current;
        this->max = rhs.max;
        this->future_damage = rhs.future_damage;
        this->sum_future_damage = rhs.sum_future_damage;
        this->ressurection_count = rhs.ressurection_count;
        return *this;
    }

    std::string ComponentHealth::get_type() const
    {
        return ComponentHealth::TYPE;
    }

    void ComponentHealth::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentHealth>(this);
    }

    void ComponentHealth::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentHealth>(this);
    }

    intrusive_ptr<ComponentBase> ComponentHealth::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentHealth>(id);
    }

    void ComponentHealth::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(current, "current", float(0));
        serializer.serialize(max, "max", float(0));
        serializer.serialize(future_damage, "future_damage");
        serializer.serialize(sum_future_damage, "sum_future_damage", float(0.0f));
        serializer.serialize(ressurection_count, "ressurection_count", int(0));
    }

    void ComponentHealth::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(current, "current", float(0));
        deserializer.deserialize(max, "max", float(0));
        deserializer.deserialize(future_damage, "future_damage");
        deserializer.deserialize(sum_future_damage, "sum_future_damage", float(0.0f));
        deserializer.deserialize(ressurection_count, "ressurection_count", int(0));
    }

    void ComponentHealth::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(current, "current", float(0));
        serializer.serialize(max, "max", float(0));
        serializer.serialize(future_damage, "future_damage");
        serializer.serialize(sum_future_damage, "sum_future_damage", float(0.0f));
        serializer.serialize(ressurection_count, "ressurection_count", int(0));
    }

    void ComponentHealth::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(current, "current", float(0));
        deserializer.deserialize(max, "max", float(0));
        deserializer.deserialize(future_damage, "future_damage");
        deserializer.deserialize(sum_future_damage, "sum_future_damage", float(0.0f));
        deserializer.deserialize(ressurection_count, "ressurection_count", int(0));
    }

} //namespace mg
