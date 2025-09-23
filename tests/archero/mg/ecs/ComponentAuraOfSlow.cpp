#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentAura.h"
#include "ComponentAuraOfSlow.h"
#include "ComponentBase.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentAuraOfSlow::TYPE("ComponentAuraOfSlow");

    ComponentAuraOfSlow::ComponentAuraOfSlow()
    {

    }

    ComponentAuraOfSlow::~ComponentAuraOfSlow()
    {
    }

    bool ComponentAuraOfSlow::operator ==(const ComponentAuraOfSlow& rhs) const
    {
        bool result = this->ComponentAura::operator ==(rhs);
        return result;
    }

    bool ComponentAuraOfSlow::operator !=(const ComponentAuraOfSlow& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentAuraOfSlow::ComponentAuraOfSlow(const ComponentAuraOfSlow& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentAuraOfSlow& ComponentAuraOfSlow::operator =(const ComponentAuraOfSlow& rhs)
    {
        this->ComponentAura::operator=(rhs);
        return *this;
    }

    std::string ComponentAuraOfSlow::get_type() const
    {
        return ComponentAuraOfSlow::TYPE;
    }

    void ComponentAuraOfSlow::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentAuraOfSlow>(this);
    }

    void ComponentAuraOfSlow::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentAuraOfSlow>(this);
    }

    intrusive_ptr<ComponentBase> ComponentAuraOfSlow::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentAuraOfSlow>(id);
    }

    void ComponentAuraOfSlow::serialize_xml(SerializerXml& serializer) const
    {
        ComponentAura::serialize_xml(serializer);
    }

    void ComponentAuraOfSlow::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentAura::deserialize_xml(deserializer);
    }

    void ComponentAuraOfSlow::serialize_json(SerializerJson& serializer) const
    {
        ComponentAura::serialize_json(serializer);
    }

    void ComponentAuraOfSlow::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentAura::deserialize_json(deserializer);
    }

} //namespace mg
