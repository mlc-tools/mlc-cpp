#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentHealingLowHp.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentHealingLowHp::TYPE("ComponentHealingLowHp");

    ComponentHealingLowHp::ComponentHealingLowHp()
    : speed(0.05)
    {

    }

    ComponentHealingLowHp::~ComponentHealingLowHp()
    {
    }

    bool ComponentHealingLowHp::operator ==(const ComponentHealingLowHp& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->speed == rhs.speed;
        return result;
    }

    bool ComponentHealingLowHp::operator !=(const ComponentHealingLowHp& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentHealingLowHp::ComponentHealingLowHp(const ComponentHealingLowHp& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentHealingLowHp& ComponentHealingLowHp::operator =(const ComponentHealingLowHp& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->speed = rhs.speed;
        return *this;
    }

    std::string ComponentHealingLowHp::get_type() const
    {
        return ComponentHealingLowHp::TYPE;
    }

    void ComponentHealingLowHp::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentHealingLowHp>(this);
    }

    void ComponentHealingLowHp::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentHealingLowHp>(this);
    }

    intrusive_ptr<ComponentBase> ComponentHealingLowHp::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentHealingLowHp>(id);
    }

    void ComponentHealingLowHp::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(speed, "speed", float(0.05));
    }

    void ComponentHealingLowHp::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(speed, "speed", float(0.05));
    }

    void ComponentHealingLowHp::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(speed, "speed", float(0.05));
    }

    void ComponentHealingLowHp::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(speed, "speed", float(0.05));
    }

} //namespace mg
