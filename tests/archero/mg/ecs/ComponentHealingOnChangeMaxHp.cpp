#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentHealingOnChangeMaxHp.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentHealingOnChangeMaxHp::TYPE("ComponentHealingOnChangeMaxHp");

    ComponentHealingOnChangeMaxHp::ComponentHealingOnChangeMaxHp()
    : current_max_hp(0)
    {

    }

    ComponentHealingOnChangeMaxHp::~ComponentHealingOnChangeMaxHp()
    {
    }

    bool ComponentHealingOnChangeMaxHp::operator ==(const ComponentHealingOnChangeMaxHp& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->current_max_hp == rhs.current_max_hp;
        return result;
    }

    bool ComponentHealingOnChangeMaxHp::operator !=(const ComponentHealingOnChangeMaxHp& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentHealingOnChangeMaxHp::ComponentHealingOnChangeMaxHp(const ComponentHealingOnChangeMaxHp& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentHealingOnChangeMaxHp& ComponentHealingOnChangeMaxHp::operator =(const ComponentHealingOnChangeMaxHp& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->current_max_hp = rhs.current_max_hp;
        return *this;
    }

    std::string ComponentHealingOnChangeMaxHp::get_type() const
    {
        return ComponentHealingOnChangeMaxHp::TYPE;
    }

    void ComponentHealingOnChangeMaxHp::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentHealingOnChangeMaxHp>(this);
    }

    void ComponentHealingOnChangeMaxHp::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentHealingOnChangeMaxHp>(this);
    }

    intrusive_ptr<ComponentBase> ComponentHealingOnChangeMaxHp::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentHealingOnChangeMaxHp>(id);
    }

    void ComponentHealingOnChangeMaxHp::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(current_max_hp, "current_max_hp", int(0));
    }

    void ComponentHealingOnChangeMaxHp::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(current_max_hp, "current_max_hp", int(0));
    }

    void ComponentHealingOnChangeMaxHp::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(current_max_hp, "current_max_hp", int(0));
    }

    void ComponentHealingOnChangeMaxHp::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(current_max_hp, "current_max_hp", int(0));
    }

} //namespace mg
