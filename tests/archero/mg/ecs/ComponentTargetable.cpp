#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentTargetable.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentTargetable::TYPE("ComponentTargetable");

    ComponentTargetable::ComponentTargetable()
    {

    }

    ComponentTargetable::~ComponentTargetable()
    {
    }

    bool ComponentTargetable::operator ==(const ComponentTargetable& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        return result;
    }

    bool ComponentTargetable::operator !=(const ComponentTargetable& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentTargetable::ComponentTargetable(const ComponentTargetable& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentTargetable& ComponentTargetable::operator =(const ComponentTargetable& rhs)
    {
        this->ComponentBase::operator=(rhs);
        return *this;
    }

    std::string ComponentTargetable::get_type() const
    {
        return ComponentTargetable::TYPE;
    }

    void ComponentTargetable::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentTargetable>(this);
    }

    void ComponentTargetable::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentTargetable>(this);
    }

    intrusive_ptr<ComponentBase> ComponentTargetable::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentTargetable>(id);
    }

    void ComponentTargetable::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
    }

    void ComponentTargetable::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
    }

    void ComponentTargetable::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
    }

    void ComponentTargetable::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
    }

} //namespace mg
