#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentSphereSpawn.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSphereSpawn::TYPE("ComponentSphereSpawn");

    ComponentSphereSpawn::ComponentSphereSpawn()
    : spawned(false)
    , count(2)
    , data(nullptr)
    {

    }

    ComponentSphereSpawn::~ComponentSphereSpawn()
    {
    }

    bool ComponentSphereSpawn::operator ==(const ComponentSphereSpawn& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->spawned == rhs.spawned;
        result = result && this->count == rhs.count;
        result = result && ((this->data == rhs.data) || (this->data != nullptr && rhs.data != nullptr && *this->data == *rhs.data));
        return result;
    }

    bool ComponentSphereSpawn::operator !=(const ComponentSphereSpawn& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSphereSpawn::ComponentSphereSpawn(const ComponentSphereSpawn& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSphereSpawn& ComponentSphereSpawn::operator =(const ComponentSphereSpawn& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->spawned = rhs.spawned;
        this->count = rhs.count;
        this->data = rhs.data;
        return *this;
    }

    std::string ComponentSphereSpawn::get_type() const
    {
        return ComponentSphereSpawn::TYPE;
    }

    void ComponentSphereSpawn::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSphereSpawn>(this);
    }

    void ComponentSphereSpawn::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSphereSpawn>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSphereSpawn::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSphereSpawn>(id);
    }

    void ComponentSphereSpawn::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(spawned, "spawned", bool(false));
        serializer.serialize(count, "count", int(2));
        serializer.serialize(data, "data");
    }

    void ComponentSphereSpawn::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(spawned, "spawned", bool(false));
        deserializer.deserialize(count, "count", int(2));
        deserializer.deserialize(data, "data");
    }

    void ComponentSphereSpawn::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(spawned, "spawned", bool(false));
        serializer.serialize(count, "count", int(2));
        serializer.serialize(data, "data");
    }

    void ComponentSphereSpawn::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(spawned, "spawned", bool(false));
        deserializer.deserialize(count, "count", int(2));
        deserializer.deserialize(data, "data");
    }

} //namespace mg
