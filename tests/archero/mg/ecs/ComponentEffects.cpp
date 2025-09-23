#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentEffects.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentEffects::TYPE("ComponentEffects");

    ComponentEffects::ComponentEffects()
    : damage_mul(1)
    , damage_mul_timer(0)
    {

    }

    ComponentEffects::~ComponentEffects()
    {
    }

    float ComponentEffects::modify_damage(float damage) const
    {
        if(this->damage_mul_timer <= 0)
        {
            return damage;
        }
        return damage * this->damage_mul;
    }

    bool ComponentEffects::operator ==(const ComponentEffects& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->damage_mul == rhs.damage_mul;
        result = result && this->damage_mul_timer == rhs.damage_mul_timer;
        return result;
    }

    bool ComponentEffects::operator !=(const ComponentEffects& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentEffects::ComponentEffects(const ComponentEffects& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentEffects& ComponentEffects::operator =(const ComponentEffects& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->damage_mul = rhs.damage_mul;
        this->damage_mul_timer = rhs.damage_mul_timer;
        return *this;
    }

    std::string ComponentEffects::get_type() const
    {
        return ComponentEffects::TYPE;
    }

    void ComponentEffects::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentEffects>(this);
    }

    void ComponentEffects::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentEffects>(this);
    }

    intrusive_ptr<ComponentBase> ComponentEffects::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentEffects>(id);
    }

    void ComponentEffects::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(damage_mul, "damage_mul", float(1));
        serializer.serialize(damage_mul_timer, "damage_mul_timer", float(0));
    }

    void ComponentEffects::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(damage_mul, "damage_mul", float(1));
        deserializer.deserialize(damage_mul_timer, "damage_mul_timer", float(0));
    }

    void ComponentEffects::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(damage_mul, "damage_mul", float(1));
        serializer.serialize(damage_mul_timer, "damage_mul_timer", float(0));
    }

    void ComponentEffects::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(damage_mul, "damage_mul", float(1));
        deserializer.deserialize(damage_mul_timer, "damage_mul_timer", float(0));
    }

} //namespace mg
