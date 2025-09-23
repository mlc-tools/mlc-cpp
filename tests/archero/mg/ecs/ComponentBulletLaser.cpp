#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentBullet.h"
#include "ComponentBulletLaser.h"
#include "ModelEcsBase.h"
#include "Segment.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentBulletLaser::TYPE("ComponentBulletLaser");

    ComponentBulletLaser::ComponentBulletLaser()
    : segment()
    , duration(0.3)
    {

    }

    ComponentBulletLaser::~ComponentBulletLaser()
    {
    }

    bool ComponentBulletLaser::operator ==(const ComponentBulletLaser& rhs) const
    {
        bool result = this->ComponentBullet::operator ==(rhs);
        result = result && this->segment == rhs.segment;
        result = result && this->duration == rhs.duration;
        return result;
    }

    bool ComponentBulletLaser::operator !=(const ComponentBulletLaser& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentBulletLaser::ComponentBulletLaser(const ComponentBulletLaser& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentBulletLaser& ComponentBulletLaser::operator =(const ComponentBulletLaser& rhs)
    {
        this->ComponentBullet::operator=(rhs);
        this->segment = rhs.segment;
        this->duration = rhs.duration;
        return *this;
    }

    std::string ComponentBulletLaser::get_type() const
    {
        return ComponentBulletLaser::TYPE;
    }

    void ComponentBulletLaser::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentBulletLaser>(this);
    }

    void ComponentBulletLaser::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentBulletLaser>(this);
    }

    intrusive_ptr<ComponentBase> ComponentBulletLaser::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentBulletLaser>(id);
    }

    void ComponentBulletLaser::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBullet::serialize_xml(serializer);
        serializer.serialize(segment, "segment");
        serializer.serialize(duration, "duration", float(0.3));
    }

    void ComponentBulletLaser::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBullet::deserialize_xml(deserializer);
        deserializer.deserialize(segment, "segment");
        deserializer.deserialize(duration, "duration", float(0.3));
    }

    void ComponentBulletLaser::serialize_json(SerializerJson& serializer) const
    {
        ComponentBullet::serialize_json(serializer);
        serializer.serialize(segment, "segment");
        serializer.serialize(duration, "duration", float(0.3));
    }

    void ComponentBulletLaser::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBullet::deserialize_json(deserializer);
        deserializer.deserialize(segment, "segment");
        deserializer.deserialize(duration, "duration", float(0.3));
    }

} //namespace mg
