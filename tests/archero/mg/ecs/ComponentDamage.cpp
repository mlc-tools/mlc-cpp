#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentDamage.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentDamage::TYPE("ComponentDamage");

    ComponentDamage::ComponentDamage()
    : attacker_id(0)
    , bullet_id(0)
    , target_id(0)
    , damage(0.0f)
    {

    }

    ComponentDamage::~ComponentDamage()
    {
    }

    bool ComponentDamage::operator ==(const ComponentDamage& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->attacker_id == rhs.attacker_id;
        result = result && this->bullet_id == rhs.bullet_id;
        result = result && this->target_id == rhs.target_id;
        result = result && this->damage == rhs.damage;
        return result;
    }

    bool ComponentDamage::operator !=(const ComponentDamage& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentDamage::ComponentDamage(const ComponentDamage& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentDamage& ComponentDamage::operator =(const ComponentDamage& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->attacker_id = rhs.attacker_id;
        this->bullet_id = rhs.bullet_id;
        this->target_id = rhs.target_id;
        this->damage = rhs.damage;
        return *this;
    }

    std::string ComponentDamage::get_type() const
    {
        return ComponentDamage::TYPE;
    }

    void ComponentDamage::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentDamage>(this);
    }

    void ComponentDamage::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentDamage>(this);
    }

    intrusive_ptr<ComponentBase> ComponentDamage::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentDamage>(id);
    }

    void ComponentDamage::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(attacker_id, "attacker_id", int(0));
        serializer.serialize(bullet_id, "bullet_id", int(0));
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(damage, "damage", float(0.0f));
    }

    void ComponentDamage::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(attacker_id, "attacker_id", int(0));
        deserializer.deserialize(bullet_id, "bullet_id", int(0));
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(damage, "damage", float(0.0f));
    }

    void ComponentDamage::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(attacker_id, "attacker_id", int(0));
        serializer.serialize(bullet_id, "bullet_id", int(0));
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(damage, "damage", float(0.0f));
    }

    void ComponentDamage::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(attacker_id, "attacker_id", int(0));
        deserializer.deserialize(bullet_id, "bullet_id", int(0));
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(damage, "damage", float(0.0f));
    }

} //namespace mg
