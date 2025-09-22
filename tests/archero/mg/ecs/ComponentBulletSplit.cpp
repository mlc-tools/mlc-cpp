#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBulletSplit.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentBulletSplit::TYPE("ComponentBulletSplit");

    ComponentBulletSplit::ComponentBulletSplit()
    : was_split(false)
    , count(0)
    , to_bullets(nullptr)
    , split_near_target_radius(0.0)
    , timer_to_split(-1)
    , split_on_remove(true)
    {

    }

    ComponentBulletSplit::~ComponentBulletSplit()
    {
    }

    bool ComponentBulletSplit::operator ==(const ComponentBulletSplit& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->was_split == rhs.was_split;
        result = result && this->count == rhs.count;
        result = result && this->split_near_target_radius == rhs.split_near_target_radius;
        result = result && this->timer_to_split == rhs.timer_to_split;
        result = result && this->split_on_remove == rhs.split_on_remove;
        return result;
    }

    bool ComponentBulletSplit::operator !=(const ComponentBulletSplit& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentBulletSplit::ComponentBulletSplit(const ComponentBulletSplit& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentBulletSplit& ComponentBulletSplit::operator =(const ComponentBulletSplit& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->was_split = rhs.was_split;
        this->count = rhs.count;
        this->to_bullets = rhs.to_bullets;
        this->split_near_target_radius = rhs.split_near_target_radius;
        this->timer_to_split = rhs.timer_to_split;
        this->split_on_remove = rhs.split_on_remove;
        return *this;
    }

    std::string ComponentBulletSplit::get_type() const
    {
        return ComponentBulletSplit::TYPE;
    }

    void ComponentBulletSplit::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentBulletSplit>(this);
    }

    void ComponentBulletSplit::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentBulletSplit>(this);
    }

    intrusive_ptr<ComponentBase> ComponentBulletSplit::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentBulletSplit>(id);
    }

    void ComponentBulletSplit::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(was_split, "was_split", bool(false));
        serializer.serialize(count, "count", int(0));
        serializer.serialize(to_bullets, "to_bullets");
        serializer.serialize(split_near_target_radius, "split_near_target_radius", float(0.0));
        serializer.serialize(timer_to_split, "timer_to_split", float(-1));
        serializer.serialize(split_on_remove, "split_on_remove", bool(true));
    }

    void ComponentBulletSplit::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(was_split, "was_split", bool(false));
        deserializer.deserialize(count, "count", int(0));
        deserializer.deserialize(to_bullets, "to_bullets");
        deserializer.deserialize(split_near_target_radius, "split_near_target_radius", float(0.0));
        deserializer.deserialize(timer_to_split, "timer_to_split", float(-1));
        deserializer.deserialize(split_on_remove, "split_on_remove", bool(true));
    }

    void ComponentBulletSplit::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(was_split, "was_split", bool(false));
        serializer.serialize(count, "count", int(0));
        serializer.serialize(to_bullets, "to_bullets");
        serializer.serialize(split_near_target_radius, "split_near_target_radius", float(0.0));
        serializer.serialize(timer_to_split, "timer_to_split", float(-1));
        serializer.serialize(split_on_remove, "split_on_remove", bool(true));
    }

    void ComponentBulletSplit::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(was_split, "was_split", bool(false));
        deserializer.deserialize(count, "count", int(0));
        deserializer.deserialize(to_bullets, "to_bullets");
        deserializer.deserialize(split_near_target_radius, "split_near_target_radius", float(0.0));
        deserializer.deserialize(timer_to_split, "timer_to_split", float(-1));
        deserializer.deserialize(split_on_remove, "split_on_remove", bool(true));
    }

} //namespace mg
