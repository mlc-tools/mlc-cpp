#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentGate.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentGate::TYPE("ComponentGate");

    ComponentGate::ComponentGate()
    : radius(64)
    , clean(false)
    {

    }

    ComponentGate::~ComponentGate()
    {
    }

    bool ComponentGate::operator ==(const ComponentGate& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->radius == rhs.radius;
        result = result && this->clean == rhs.clean;
        return result;
    }

    bool ComponentGate::operator !=(const ComponentGate& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentGate::ComponentGate(const ComponentGate& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentGate& ComponentGate::operator =(const ComponentGate& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->radius = rhs.radius;
        this->clean = rhs.clean;
        return *this;
    }

    std::string ComponentGate::get_type() const
    {
        return ComponentGate::TYPE;
    }

    void ComponentGate::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentGate>(this);
    }

    void ComponentGate::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentGate>(this);
    }

    intrusive_ptr<ComponentBase> ComponentGate::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentGate>(id);
    }

    void ComponentGate::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(radius, "radius", float(64));
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentGate::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(radius, "radius", float(64));
        deserializer.deserialize(clean, "clean", bool(false));
    }

    void ComponentGate::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(radius, "radius", float(64));
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentGate::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(radius, "radius", float(64));
        deserializer.deserialize(clean, "clean", bool(false));
    }

} //namespace mg
