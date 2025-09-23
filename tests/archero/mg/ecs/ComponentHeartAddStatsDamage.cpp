#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentHeartAddStats.h"
#include "ComponentHeartAddStatsDamage.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentHeartAddStatsDamage::TYPE("ComponentHeartAddStatsDamage");

    ComponentHeartAddStatsDamage::ComponentHeartAddStatsDamage()
    {

    }

    ComponentHeartAddStatsDamage::~ComponentHeartAddStatsDamage()
    {
    }

    bool ComponentHeartAddStatsDamage::operator ==(const ComponentHeartAddStatsDamage& rhs) const
    {
        bool result = this->ComponentHeartAddStats::operator ==(rhs);
        return result;
    }

    bool ComponentHeartAddStatsDamage::operator !=(const ComponentHeartAddStatsDamage& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentHeartAddStatsDamage::ComponentHeartAddStatsDamage(const ComponentHeartAddStatsDamage& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentHeartAddStatsDamage& ComponentHeartAddStatsDamage::operator =(const ComponentHeartAddStatsDamage& rhs)
    {
        this->ComponentHeartAddStats::operator=(rhs);
        return *this;
    }

    std::string ComponentHeartAddStatsDamage::get_type() const
    {
        return ComponentHeartAddStatsDamage::TYPE;
    }

    void ComponentHeartAddStatsDamage::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentHeartAddStatsDamage>(this);
    }

    void ComponentHeartAddStatsDamage::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentHeartAddStatsDamage>(this);
    }

    intrusive_ptr<ComponentBase> ComponentHeartAddStatsDamage::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentHeartAddStatsDamage>(id);
    }

    void ComponentHeartAddStatsDamage::serialize_xml(SerializerXml& serializer) const
    {
        ComponentHeartAddStats::serialize_xml(serializer);
    }

    void ComponentHeartAddStatsDamage::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentHeartAddStats::deserialize_xml(deserializer);
    }

    void ComponentHeartAddStatsDamage::serialize_json(SerializerJson& serializer) const
    {
        ComponentHeartAddStats::serialize_json(serializer);
    }

    void ComponentHeartAddStatsDamage::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentHeartAddStats::deserialize_json(deserializer);
    }

} //namespace mg
