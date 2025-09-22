#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentAura.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentAura::TYPE("ComponentAura");

    ComponentAura::ComponentAura()
    : name("")
    , initialized(false)
    , radius(0.0)
    {

    }

    ComponentAura::~ComponentAura()
    {
    }

    bool ComponentAura::operator ==(const ComponentAura& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->name == rhs.name;
        result = result && this->initialized == rhs.initialized;
        result = result && this->radius == rhs.radius;
        return result;
    }

    bool ComponentAura::operator !=(const ComponentAura& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentAura::ComponentAura(const ComponentAura& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentAura& ComponentAura::operator =(const ComponentAura& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->name = rhs.name;
        this->initialized = rhs.initialized;
        this->radius = rhs.radius;
        return *this;
    }

    std::string ComponentAura::get_type() const
    {
        return ComponentAura::TYPE;
    }

    void ComponentAura::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentAura>(this);
    }

    void ComponentAura::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentAura>(this);
    }

    intrusive_ptr<ComponentBase> ComponentAura::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentAura>(id);
    }

    void ComponentAura::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(radius, "radius", float(0.0));
    }

    void ComponentAura::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(radius, "radius", float(0.0));
    }

    void ComponentAura::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(radius, "radius", float(0.0));
    }

    void ComponentAura::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(radius, "radius", float(0.0));
    }

} //namespace mg
