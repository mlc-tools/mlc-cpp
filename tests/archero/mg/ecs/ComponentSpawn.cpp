#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentSpawn.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSpawn::TYPE("ComponentSpawn");

    ComponentSpawn::ComponentSpawn()
    : unit(nullptr)
    , position()
    , timer(2)
    , create_marker(true)
    , clean(false)
    {

    }

    ComponentSpawn::~ComponentSpawn()
    {
    }

    bool ComponentSpawn::operator ==(const ComponentSpawn& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->position == rhs.position;
        result = result && this->timer == rhs.timer;
        result = result && this->create_marker == rhs.create_marker;
        result = result && this->clean == rhs.clean;
        return result;
    }

    bool ComponentSpawn::operator !=(const ComponentSpawn& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSpawn::ComponentSpawn(const ComponentSpawn& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSpawn& ComponentSpawn::operator =(const ComponentSpawn& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->unit = rhs.unit;
        this->position = rhs.position;
        this->timer = rhs.timer;
        this->create_marker = rhs.create_marker;
        this->clean = rhs.clean;
        return *this;
    }

    std::string ComponentSpawn::get_type() const
    {
        return ComponentSpawn::TYPE;
    }

    void ComponentSpawn::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSpawn>(this);
    }

    void ComponentSpawn::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSpawn>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSpawn::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSpawn>(id);
    }

    void ComponentSpawn::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(unit, "unit");
        serializer.serialize(position, "position");
        serializer.serialize(timer, "timer", float(2));
        serializer.serialize(create_marker, "create_marker", bool(true));
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentSpawn::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(unit, "unit");
        deserializer.deserialize(position, "position");
        deserializer.deserialize(timer, "timer", float(2));
        deserializer.deserialize(create_marker, "create_marker", bool(true));
        deserializer.deserialize(clean, "clean", bool(false));
    }

    void ComponentSpawn::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(unit, "unit");
        serializer.serialize(position, "position");
        serializer.serialize(timer, "timer", float(2));
        serializer.serialize(create_marker, "create_marker", bool(true));
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentSpawn::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(unit, "unit");
        deserializer.deserialize(position, "position");
        deserializer.deserialize(timer, "timer", float(2));
        deserializer.deserialize(create_marker, "create_marker", bool(true));
        deserializer.deserialize(clean, "clean", bool(false));
    }

} //namespace mg
