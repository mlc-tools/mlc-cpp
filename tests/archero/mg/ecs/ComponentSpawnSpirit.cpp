#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentSpawnSpirit.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSpawnSpirit::TYPE("ComponentSpawnSpirit");

    ComponentSpawnSpirit::ComponentSpawnSpirit()
    : spirit(nullptr)
    {

    }

    ComponentSpawnSpirit::~ComponentSpawnSpirit()
    {
    }

    bool ComponentSpawnSpirit::operator ==(const ComponentSpawnSpirit& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && ((this->spirit == rhs.spirit) || (this->spirit != nullptr && rhs.spirit != nullptr && *this->spirit == *rhs.spirit));
        return result;
    }

    bool ComponentSpawnSpirit::operator !=(const ComponentSpawnSpirit& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSpawnSpirit::ComponentSpawnSpirit(const ComponentSpawnSpirit& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSpawnSpirit& ComponentSpawnSpirit::operator =(const ComponentSpawnSpirit& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->spirit = rhs.spirit;
        return *this;
    }

    std::string ComponentSpawnSpirit::get_type() const
    {
        return ComponentSpawnSpirit::TYPE;
    }

    void ComponentSpawnSpirit::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSpawnSpirit>(this);
    }

    void ComponentSpawnSpirit::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSpawnSpirit>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSpawnSpirit::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSpawnSpirit>(id);
    }

    void ComponentSpawnSpirit::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(spirit, "spirit");
    }

    void ComponentSpawnSpirit::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(spirit, "spirit");
    }

    void ComponentSpawnSpirit::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(spirit, "spirit");
    }

    void ComponentSpawnSpirit::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(spirit, "spirit");
    }

} //namespace mg
