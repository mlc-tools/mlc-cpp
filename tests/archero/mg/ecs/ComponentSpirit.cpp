#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentSpirit.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSpirit::TYPE("ComponentSpirit");

    ComponentSpirit::ComponentSpirit()
    : initialized(false)
    , speed_random_t(1)
    , base_point()
    {

    }

    ComponentSpirit::~ComponentSpirit()
    {
    }

    bool ComponentSpirit::operator ==(const ComponentSpirit& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->initialized == rhs.initialized;
        result = result && this->speed_random_t == rhs.speed_random_t;
        result = result && this->base_point == rhs.base_point;
        return result;
    }

    bool ComponentSpirit::operator !=(const ComponentSpirit& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSpirit::ComponentSpirit(const ComponentSpirit& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSpirit& ComponentSpirit::operator =(const ComponentSpirit& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->initialized = rhs.initialized;
        this->speed_random_t = rhs.speed_random_t;
        this->base_point = rhs.base_point;
        return *this;
    }

    std::string ComponentSpirit::get_type() const
    {
        return ComponentSpirit::TYPE;
    }

    void ComponentSpirit::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSpirit>(this);
    }

    void ComponentSpirit::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSpirit>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSpirit::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSpirit>(id);
    }

    void ComponentSpirit::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(speed_random_t, "speed_random_t", float(1));
        serializer.serialize(base_point, "base_point");
    }

    void ComponentSpirit::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(speed_random_t, "speed_random_t", float(1));
        deserializer.deserialize(base_point, "base_point");
    }

    void ComponentSpirit::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(speed_random_t, "speed_random_t", float(1));
        serializer.serialize(base_point, "base_point");
    }

    void ComponentSpirit::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(speed_random_t, "speed_random_t", float(1));
        deserializer.deserialize(base_point, "base_point");
    }

} //namespace mg
