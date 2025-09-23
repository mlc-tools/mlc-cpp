#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentShield.h"
#include "ComponentShieldType.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentShield::TYPE("ComponentShield");

    ComponentShield::ComponentShield()
    : initialized(false)
    , active(true)
    , timer(0)
    , activate_type()
    {

    }

    ComponentShield::~ComponentShield()
    {
    }

    bool ComponentShield::operator ==(const ComponentShield& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->initialized == rhs.initialized;
        result = result && this->active == rhs.active;
        result = result && this->timer == rhs.timer;
        result = result && this->activate_type == rhs.activate_type;
        return result;
    }

    bool ComponentShield::operator !=(const ComponentShield& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentShield::ComponentShield(const ComponentShield& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentShield& ComponentShield::operator =(const ComponentShield& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->initialized = rhs.initialized;
        this->active = rhs.active;
        this->timer = rhs.timer;
        this->activate_type = rhs.activate_type;
        return *this;
    }

    std::string ComponentShield::get_type() const
    {
        return ComponentShield::TYPE;
    }

    void ComponentShield::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentShield>(this);
    }

    void ComponentShield::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentShield>(this);
    }

    intrusive_ptr<ComponentBase> ComponentShield::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentShield>(id);
    }

    void ComponentShield::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(active, "active", bool(true));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(activate_type, "activate_type");
    }

    void ComponentShield::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(active, "active", bool(true));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(activate_type, "activate_type");
    }

    void ComponentShield::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(active, "active", bool(true));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(activate_type, "activate_type");
    }

    void ComponentShield::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(active, "active", bool(true));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(activate_type, "activate_type");
    }

} //namespace mg
