#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentBusy.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentBusy::TYPE("ComponentBusy");

    ComponentBusy::ComponentBusy()
    : is_busy(false)
    {

    }

    ComponentBusy::~ComponentBusy()
    {
    }

    bool ComponentBusy::operator ==(const ComponentBusy& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->is_busy == rhs.is_busy;
        return result;
    }

    bool ComponentBusy::operator !=(const ComponentBusy& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentBusy::ComponentBusy(const ComponentBusy& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentBusy& ComponentBusy::operator =(const ComponentBusy& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->is_busy = rhs.is_busy;
        return *this;
    }

    std::string ComponentBusy::get_type() const
    {
        return ComponentBusy::TYPE;
    }

    void ComponentBusy::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentBusy>(this);
    }

    void ComponentBusy::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentBusy>(this);
    }

    intrusive_ptr<ComponentBase> ComponentBusy::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentBusy>(id);
    }

    void ComponentBusy::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(is_busy, "is_busy", bool(false));
    }

    void ComponentBusy::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(is_busy, "is_busy", bool(false));
    }

    void ComponentBusy::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(is_busy, "is_busy", bool(false));
    }

    void ComponentBusy::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(is_busy, "is_busy", bool(false));
    }

} //namespace mg
