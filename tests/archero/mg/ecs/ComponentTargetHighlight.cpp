#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentTargetHighlight.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentTargetHighlight::TYPE("ComponentTargetHighlight");

    ComponentTargetHighlight::ComponentTargetHighlight()
    : target_id(0)
    {

    }

    ComponentTargetHighlight::~ComponentTargetHighlight()
    {
    }

    bool ComponentTargetHighlight::operator ==(const ComponentTargetHighlight& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->target_id == rhs.target_id;
        return result;
    }

    bool ComponentTargetHighlight::operator !=(const ComponentTargetHighlight& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentTargetHighlight::ComponentTargetHighlight(const ComponentTargetHighlight& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentTargetHighlight& ComponentTargetHighlight::operator =(const ComponentTargetHighlight& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->target_id = rhs.target_id;
        return *this;
    }

    std::string ComponentTargetHighlight::get_type() const
    {
        return ComponentTargetHighlight::TYPE;
    }

    void ComponentTargetHighlight::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentTargetHighlight>(this);
    }

    void ComponentTargetHighlight::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentTargetHighlight>(this);
    }

    intrusive_ptr<ComponentBase> ComponentTargetHighlight::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentTargetHighlight>(id);
    }

    void ComponentTargetHighlight::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(target_id, "target_id", int(0));
    }

    void ComponentTargetHighlight::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
    }

    void ComponentTargetHighlight::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(target_id, "target_id", int(0));
    }

    void ComponentTargetHighlight::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
    }

} //namespace mg
