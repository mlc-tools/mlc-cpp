#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentAuraDamage.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentAuraDamage::TYPE("ComponentAuraDamage");

    ComponentAuraDamage::ComponentAuraDamage()
    : damage_mul(0.2)
    , damaged_entities()
    {

    }

    ComponentAuraDamage::~ComponentAuraDamage()
    {
    }

    bool ComponentAuraDamage::operator ==(const ComponentAuraDamage& rhs) const
    {
        bool result = this->ComponentAura::operator ==(rhs);
        result = result && this->damage_mul == rhs.damage_mul;
        result = result && this->damaged_entities == rhs.damaged_entities;
        return result;
    }

    bool ComponentAuraDamage::operator !=(const ComponentAuraDamage& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentAuraDamage::ComponentAuraDamage(const ComponentAuraDamage& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentAuraDamage& ComponentAuraDamage::operator =(const ComponentAuraDamage& rhs)
    {
        this->ComponentAura::operator=(rhs);
        this->damage_mul = rhs.damage_mul;
        this->damaged_entities = rhs.damaged_entities;
        return *this;
    }

    std::string ComponentAuraDamage::get_type() const
    {
        return ComponentAuraDamage::TYPE;
    }

    void ComponentAuraDamage::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentAuraDamage>(this);
    }

    void ComponentAuraDamage::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentAuraDamage>(this);
    }

    intrusive_ptr<ComponentBase> ComponentAuraDamage::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentAuraDamage>(id);
    }

    void ComponentAuraDamage::serialize_xml(SerializerXml& serializer) const
    {
        ComponentAura::serialize_xml(serializer);
        serializer.serialize(damage_mul, "damage_mul", float(0.2));
        serializer.serialize(damaged_entities, "damaged_entities");
    }

    void ComponentAuraDamage::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentAura::deserialize_xml(deserializer);
        deserializer.deserialize(damage_mul, "damage_mul", float(0.2));
        deserializer.deserialize(damaged_entities, "damaged_entities");
    }

    void ComponentAuraDamage::serialize_json(SerializerJson& serializer) const
    {
        ComponentAura::serialize_json(serializer);
        serializer.serialize(damage_mul, "damage_mul", float(0.2));
        serializer.serialize(damaged_entities, "damaged_entities");
    }

    void ComponentAuraDamage::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentAura::deserialize_json(deserializer);
        deserializer.deserialize(damage_mul, "damage_mul", float(0.2));
        deserializer.deserialize(damaged_entities, "damaged_entities");
    }

} //namespace mg
