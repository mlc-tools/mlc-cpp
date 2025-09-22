#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentSphere.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSphere::TYPE("ComponentSphere");

    ComponentSphere::ComponentSphere()
    : hero_id(0)
    , angle(0.0)
    , radius(200)
    , speed(3.14)
    , ignore_targets()
    {

    }

    ComponentSphere::~ComponentSphere()
    {
    }

    bool ComponentSphere::operator ==(const ComponentSphere& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->hero_id == rhs.hero_id;
        result = result && this->angle == rhs.angle;
        result = result && this->radius == rhs.radius;
        result = result && this->speed == rhs.speed;
        result = result && this->ignore_targets == rhs.ignore_targets;
        return result;
    }

    bool ComponentSphere::operator !=(const ComponentSphere& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSphere::ComponentSphere(const ComponentSphere& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSphere& ComponentSphere::operator =(const ComponentSphere& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->hero_id = rhs.hero_id;
        this->angle = rhs.angle;
        this->radius = rhs.radius;
        this->speed = rhs.speed;
        this->ignore_targets = rhs.ignore_targets;
        return *this;
    }

    std::string ComponentSphere::get_type() const
    {
        return ComponentSphere::TYPE;
    }

    void ComponentSphere::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSphere>(this);
    }

    void ComponentSphere::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSphere>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSphere::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSphere>(id);
    }

    void ComponentSphere::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(hero_id, "hero_id", int(0));
        serializer.serialize(angle, "angle", float(0.0));
        serializer.serialize(radius, "radius", float(200));
        serializer.serialize(speed, "speed", float(3.14));
        serializer.serialize(ignore_targets, "ignore_targets");
    }

    void ComponentSphere::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(hero_id, "hero_id", int(0));
        deserializer.deserialize(angle, "angle", float(0.0));
        deserializer.deserialize(radius, "radius", float(200));
        deserializer.deserialize(speed, "speed", float(3.14));
        deserializer.deserialize(ignore_targets, "ignore_targets");
    }

    void ComponentSphere::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(hero_id, "hero_id", int(0));
        serializer.serialize(angle, "angle", float(0.0));
        serializer.serialize(radius, "radius", float(200));
        serializer.serialize(speed, "speed", float(3.14));
        serializer.serialize(ignore_targets, "ignore_targets");
    }

    void ComponentSphere::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(hero_id, "hero_id", int(0));
        deserializer.deserialize(angle, "angle", float(0.0));
        deserializer.deserialize(radius, "radius", float(200));
        deserializer.deserialize(speed, "speed", float(3.14));
        deserializer.deserialize(ignore_targets, "ignore_targets");
    }

} //namespace mg
