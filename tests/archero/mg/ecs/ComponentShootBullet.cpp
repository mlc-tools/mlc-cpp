#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentShootBullet.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentShootBullet::TYPE("ComponentShootBullet");

    ComponentShootBullet::ComponentShootBullet()
    {

    }

    ComponentShootBullet::~ComponentShootBullet()
    {
    }

    bool ComponentShootBullet::operator ==(const ComponentShootBullet& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        return result;
    }

    bool ComponentShootBullet::operator !=(const ComponentShootBullet& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentShootBullet::ComponentShootBullet(const ComponentShootBullet& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentShootBullet& ComponentShootBullet::operator =(const ComponentShootBullet& rhs)
    {
        this->ComponentBase::operator=(rhs);
        return *this;
    }

    std::string ComponentShootBullet::get_type() const
    {
        return ComponentShootBullet::TYPE;
    }

    void ComponentShootBullet::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentShootBullet>(this);
    }

    void ComponentShootBullet::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentShootBullet>(this);
    }

    intrusive_ptr<ComponentBase> ComponentShootBullet::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentShootBullet>(id);
    }

    void ComponentShootBullet::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
    }

    void ComponentShootBullet::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
    }

    void ComponentShootBullet::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
    }

    void ComponentShootBullet::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
    }

} //namespace mg
