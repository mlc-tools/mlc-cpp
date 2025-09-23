#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ModelEcsBase.h"
#include "Transform.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Transform::TYPE("Transform");

    Transform::Transform()
    : prev_position()
    , position()
    , z(0)
    , rotation(-1)
    {

    }

    Transform::~Transform()
    {
    }

    void Transform::set(const Vector& position)
    {
        prev_position = this->position;
        this->position = position;
    }

    bool Transform::operator ==(const Transform& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->prev_position == rhs.prev_position;
        result = result && this->position == rhs.position;
        result = result && this->z == rhs.z;
        result = result && this->rotation == rhs.rotation;
        return result;
    }

    bool Transform::operator !=(const Transform& rhs) const
    {
        return !(*this == rhs);
    }

    Transform::Transform(const Transform& rhs)
    {
        this->operator=(rhs);
    }

    const Transform& Transform::operator =(const Transform& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->prev_position = rhs.prev_position;
        this->position = rhs.position;
        this->z = rhs.z;
        this->rotation = rhs.rotation;
        return *this;
    }

    std::string Transform::get_type() const
    {
        return Transform::TYPE;
    }

    void Transform::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<Transform>(this);
    }

    void Transform::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<Transform>(this);
    }

    intrusive_ptr<ComponentBase> Transform::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<Transform>(id);
    }

    void Transform::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(prev_position, "prev_position");
        serializer.serialize(position, "position");
        serializer.serialize(z, "z", int(0));
        serializer.serialize(rotation, "rotation", float(-1));
    }

    void Transform::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(prev_position, "prev_position");
        deserializer.deserialize(position, "position");
        deserializer.deserialize(z, "z", int(0));
        deserializer.deserialize(rotation, "rotation", float(-1));
    }

    void Transform::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(prev_position, "prev_position");
        serializer.serialize(position, "position");
        serializer.serialize(z, "z", int(0));
        serializer.serialize(rotation, "rotation", float(-1));
    }

    void Transform::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(prev_position, "prev_position");
        deserializer.deserialize(position, "position");
        deserializer.deserialize(z, "z", int(0));
        deserializer.deserialize(rotation, "rotation", float(-1));
    }

} //namespace mg
