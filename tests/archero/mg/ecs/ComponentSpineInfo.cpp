#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentSpineInfo.h"
#include "ModelEcsBase.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSpineInfo::TYPE("ComponentSpineInfo");

    ComponentSpineInfo::ComponentSpineInfo()
    {

    }

    ComponentSpineInfo::~ComponentSpineInfo()
    {
    }

    bool ComponentSpineInfo::operator ==(const ComponentSpineInfo& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        return result;
    }

    bool ComponentSpineInfo::operator !=(const ComponentSpineInfo& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSpineInfo::ComponentSpineInfo(const ComponentSpineInfo& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSpineInfo& ComponentSpineInfo::operator =(const ComponentSpineInfo& rhs)
    {
        this->ComponentBase::operator=(rhs);
        return *this;
    }

    std::string ComponentSpineInfo::get_type() const
    {
        return ComponentSpineInfo::TYPE;
    }

    void ComponentSpineInfo::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSpineInfo>(this);
    }

    void ComponentSpineInfo::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSpineInfo>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSpineInfo::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSpineInfo>(id);
    }

    void ComponentSpineInfo::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
    }

    void ComponentSpineInfo::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
    }

    void ComponentSpineInfo::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
    }

    void ComponentSpineInfo::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
    }

} //namespace mg
