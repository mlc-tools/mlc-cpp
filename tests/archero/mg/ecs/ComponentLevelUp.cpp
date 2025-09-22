#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentLevelUp.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentLevelUp::TYPE("ComponentLevelUp");

    ComponentLevelUp::ComponentLevelUp()
    : level(0)
    , exp_current(0)
    {

    }

    ComponentLevelUp::~ComponentLevelUp()
    {
    }

    bool ComponentLevelUp::operator ==(const ComponentLevelUp& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->level == rhs.level;
        result = result && this->exp_current == rhs.exp_current;
        return result;
    }

    bool ComponentLevelUp::operator !=(const ComponentLevelUp& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentLevelUp::ComponentLevelUp(const ComponentLevelUp& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentLevelUp& ComponentLevelUp::operator =(const ComponentLevelUp& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->level = rhs.level;
        this->exp_current = rhs.exp_current;
        return *this;
    }

    std::string ComponentLevelUp::get_type() const
    {
        return ComponentLevelUp::TYPE;
    }

    void ComponentLevelUp::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentLevelUp>(this);
    }

    void ComponentLevelUp::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentLevelUp>(this);
    }

    intrusive_ptr<ComponentBase> ComponentLevelUp::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentLevelUp>(id);
    }

    void ComponentLevelUp::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(level, "level", int(0));
        serializer.serialize(exp_current, "exp_current", int(0));
    }

    void ComponentLevelUp::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(level, "level", int(0));
        deserializer.deserialize(exp_current, "exp_current", int(0));
    }

    void ComponentLevelUp::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(level, "level", int(0));
        serializer.serialize(exp_current, "exp_current", int(0));
    }

    void ComponentLevelUp::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(level, "level", int(0));
        deserializer.deserialize(exp_current, "exp_current", int(0));
    }

} //namespace mg
