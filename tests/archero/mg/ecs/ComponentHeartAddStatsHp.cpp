#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentHeartAddStats.h"
#include "ComponentHeartAddStatsHp.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentHeartAddStatsHp::TYPE("ComponentHeartAddStatsHp");

    ComponentHeartAddStatsHp::ComponentHeartAddStatsHp()
    {

    }

    ComponentHeartAddStatsHp::~ComponentHeartAddStatsHp()
    {
    }

    bool ComponentHeartAddStatsHp::operator ==(const ComponentHeartAddStatsHp& rhs) const
    {
        bool result = this->ComponentHeartAddStats::operator ==(rhs);
        return result;
    }

    bool ComponentHeartAddStatsHp::operator !=(const ComponentHeartAddStatsHp& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentHeartAddStatsHp::ComponentHeartAddStatsHp(const ComponentHeartAddStatsHp& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentHeartAddStatsHp& ComponentHeartAddStatsHp::operator =(const ComponentHeartAddStatsHp& rhs)
    {
        this->ComponentHeartAddStats::operator=(rhs);
        return *this;
    }

    std::string ComponentHeartAddStatsHp::get_type() const
    {
        return ComponentHeartAddStatsHp::TYPE;
    }

    void ComponentHeartAddStatsHp::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentHeartAddStatsHp>(this);
    }

    void ComponentHeartAddStatsHp::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentHeartAddStatsHp>(this);
    }

    intrusive_ptr<ComponentBase> ComponentHeartAddStatsHp::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentHeartAddStatsHp>(id);
    }

    void ComponentHeartAddStatsHp::serialize_xml(SerializerXml& serializer) const
    {
        ComponentHeartAddStats::serialize_xml(serializer);
    }

    void ComponentHeartAddStatsHp::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentHeartAddStats::deserialize_xml(deserializer);
    }

    void ComponentHeartAddStatsHp::serialize_json(SerializerJson& serializer) const
    {
        ComponentHeartAddStats::serialize_json(serializer);
    }

    void ComponentHeartAddStatsHp::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentHeartAddStats::deserialize_json(deserializer);
    }

} //namespace mg
