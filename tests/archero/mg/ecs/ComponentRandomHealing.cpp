#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentRandomHealing.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentRandomHealing::TYPE("ComponentRandomHealing");

    ComponentRandomHealing::ComponentRandomHealing()
    : timer(1)
    , chance(0.01)
    {

    }

    ComponentRandomHealing::~ComponentRandomHealing()
    {
    }

    bool ComponentRandomHealing::operator ==(const ComponentRandomHealing& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->timer == rhs.timer;
        result = result && this->chance == rhs.chance;
        return result;
    }

    bool ComponentRandomHealing::operator !=(const ComponentRandomHealing& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentRandomHealing::ComponentRandomHealing(const ComponentRandomHealing& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentRandomHealing& ComponentRandomHealing::operator =(const ComponentRandomHealing& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->timer = rhs.timer;
        this->chance = rhs.chance;
        return *this;
    }

    std::string ComponentRandomHealing::get_type() const
    {
        return ComponentRandomHealing::TYPE;
    }

    void ComponentRandomHealing::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentRandomHealing>(this);
    }

    void ComponentRandomHealing::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentRandomHealing>(this);
    }

    intrusive_ptr<ComponentBase> ComponentRandomHealing::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentRandomHealing>(id);
    }

    void ComponentRandomHealing::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(timer, "timer", float(1));
        serializer.serialize(chance, "chance", float(0.01));
    }

    void ComponentRandomHealing::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(timer, "timer", float(1));
        deserializer.deserialize(chance, "chance", float(0.01));
    }

    void ComponentRandomHealing::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(timer, "timer", float(1));
        serializer.serialize(chance, "chance", float(0.01));
    }

    void ComponentRandomHealing::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(timer, "timer", float(1));
        deserializer.deserialize(chance, "chance", float(0.01));
    }

} //namespace mg
