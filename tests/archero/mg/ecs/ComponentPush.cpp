#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentPush.h"
#include "ModelEcsBase.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentPush::TYPE("ComponentPush");

    ComponentPush::ComponentPush()
    : direction()
    , duration(0.1)
    , speed(0.0f)
    {

    }

    ComponentPush::~ComponentPush()
    {
    }

    bool ComponentPush::operator ==(const ComponentPush& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->direction == rhs.direction;
        result = result && this->duration == rhs.duration;
        result = result && this->speed == rhs.speed;
        return result;
    }

    bool ComponentPush::operator !=(const ComponentPush& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentPush::ComponentPush(const ComponentPush& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentPush& ComponentPush::operator =(const ComponentPush& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->direction = rhs.direction;
        this->duration = rhs.duration;
        this->speed = rhs.speed;
        return *this;
    }

    std::string ComponentPush::get_type() const
    {
        return ComponentPush::TYPE;
    }

    void ComponentPush::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentPush>(this);
    }

    void ComponentPush::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentPush>(this);
    }

    intrusive_ptr<ComponentBase> ComponentPush::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentPush>(id);
    }

    void ComponentPush::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(direction, "direction");
        serializer.serialize(duration, "duration", float(0.1));
        serializer.serialize(speed, "speed", float(0.0f));
    }

    void ComponentPush::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(direction, "direction");
        deserializer.deserialize(duration, "duration", float(0.1));
        deserializer.deserialize(speed, "speed", float(0.0f));
    }

    void ComponentPush::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(direction, "direction");
        serializer.serialize(duration, "duration", float(0.1));
        serializer.serialize(speed, "speed", float(0.0f));
    }

    void ComponentPush::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(direction, "direction");
        deserializer.deserialize(duration, "duration", float(0.1));
        deserializer.deserialize(speed, "speed", float(0.0f));
    }

} //namespace mg
