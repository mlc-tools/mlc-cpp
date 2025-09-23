#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentVampire.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentVampire::TYPE("ComponentVampire");

    ComponentVampire::ComponentVampire()
    : chance(0.1)
    , heal_percent(0.05)
    {

    }

    ComponentVampire::~ComponentVampire()
    {
    }

    bool ComponentVampire::operator ==(const ComponentVampire& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->chance == rhs.chance;
        result = result && this->heal_percent == rhs.heal_percent;
        return result;
    }

    bool ComponentVampire::operator !=(const ComponentVampire& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentVampire::ComponentVampire(const ComponentVampire& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentVampire& ComponentVampire::operator =(const ComponentVampire& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->chance = rhs.chance;
        this->heal_percent = rhs.heal_percent;
        return *this;
    }

    std::string ComponentVampire::get_type() const
    {
        return ComponentVampire::TYPE;
    }

    void ComponentVampire::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentVampire>(this);
    }

    void ComponentVampire::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentVampire>(this);
    }

    intrusive_ptr<ComponentBase> ComponentVampire::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentVampire>(id);
    }

    void ComponentVampire::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(chance, "chance", float(0.1));
        serializer.serialize(heal_percent, "heal_percent", float(0.05));
    }

    void ComponentVampire::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(chance, "chance", float(0.1));
        deserializer.deserialize(heal_percent, "heal_percent", float(0.05));
    }

    void ComponentVampire::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(chance, "chance", float(0.1));
        serializer.serialize(heal_percent, "heal_percent", float(0.05));
    }

    void ComponentVampire::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(chance, "chance", float(0.1));
        deserializer.deserialize(heal_percent, "heal_percent", float(0.05));
    }

} //namespace mg
