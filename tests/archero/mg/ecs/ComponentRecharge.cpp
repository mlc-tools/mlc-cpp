#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentRecharge.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentRecharge::TYPE("ComponentRecharge");

    ComponentRecharge::ComponentRecharge()
    : recharge(0.0f)
    , attack_animation_timer(0)
    {

    }

    ComponentRecharge::~ComponentRecharge()
    {
    }

    bool ComponentRecharge::operator ==(const ComponentRecharge& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->recharge == rhs.recharge;
        result = result && this->attack_animation_timer == rhs.attack_animation_timer;
        return result;
    }

    bool ComponentRecharge::operator !=(const ComponentRecharge& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentRecharge::ComponentRecharge(const ComponentRecharge& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentRecharge& ComponentRecharge::operator =(const ComponentRecharge& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->recharge = rhs.recharge;
        this->attack_animation_timer = rhs.attack_animation_timer;
        return *this;
    }

    std::string ComponentRecharge::get_type() const
    {
        return ComponentRecharge::TYPE;
    }

    void ComponentRecharge::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentRecharge>(this);
    }

    void ComponentRecharge::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentRecharge>(this);
    }

    intrusive_ptr<ComponentBase> ComponentRecharge::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentRecharge>(id);
    }

    void ComponentRecharge::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(recharge, "recharge", float(0.0f));
        serializer.serialize(attack_animation_timer, "attack_animation_timer", float(0));
    }

    void ComponentRecharge::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(recharge, "recharge", float(0.0f));
        deserializer.deserialize(attack_animation_timer, "attack_animation_timer", float(0));
    }

    void ComponentRecharge::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(recharge, "recharge", float(0.0f));
        serializer.serialize(attack_animation_timer, "attack_animation_timer", float(0));
    }

    void ComponentRecharge::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(recharge, "recharge", float(0.0f));
        deserializer.deserialize(attack_animation_timer, "attack_animation_timer", float(0));
    }

} //namespace mg
