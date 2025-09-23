#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentBody.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentBody::TYPE("ComponentBody");

    ComponentBody::ComponentBody()
    : size(0.0f)
    {

    }

    ComponentBody::~ComponentBody()
    {
    }

    bool ComponentBody::operator ==(const ComponentBody& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->size == rhs.size;
        return result;
    }

    bool ComponentBody::operator !=(const ComponentBody& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentBody::ComponentBody(const ComponentBody& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentBody& ComponentBody::operator =(const ComponentBody& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->size = rhs.size;
        return *this;
    }

    std::string ComponentBody::get_type() const
    {
        return ComponentBody::TYPE;
    }

    void ComponentBody::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentBody>(this);
    }

    void ComponentBody::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentBody>(this);
    }

    intrusive_ptr<ComponentBase> ComponentBody::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentBody>(id);
    }

    void ComponentBody::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(size, "size", float(0.0f));
    }

    void ComponentBody::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(size, "size", float(0.0f));
    }

    void ComponentBody::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(size, "size", float(0.0f));
    }

    void ComponentBody::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(size, "size", float(0.0f));
    }

} //namespace mg
