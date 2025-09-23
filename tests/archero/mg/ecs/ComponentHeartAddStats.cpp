#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentHeartAddStats.h"
#include "DataStatUpgrade.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentHeartAddStats::TYPE("ComponentHeartAddStats");

    ComponentHeartAddStats::ComponentHeartAddStats()
    : percent(1)
    , stat_upgrade(nullptr)
    {

    }

    ComponentHeartAddStats::~ComponentHeartAddStats()
    {
    }

    bool ComponentHeartAddStats::operator ==(const ComponentHeartAddStats& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->percent == rhs.percent;
        result = result && ((this->stat_upgrade == rhs.stat_upgrade) || (this->stat_upgrade != nullptr && rhs.stat_upgrade != nullptr && *this->stat_upgrade == *rhs.stat_upgrade));
        return result;
    }

    bool ComponentHeartAddStats::operator !=(const ComponentHeartAddStats& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentHeartAddStats::ComponentHeartAddStats(const ComponentHeartAddStats& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentHeartAddStats& ComponentHeartAddStats::operator =(const ComponentHeartAddStats& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->percent = rhs.percent;
        this->stat_upgrade = rhs.stat_upgrade;
        return *this;
    }

    std::string ComponentHeartAddStats::get_type() const
    {
        return ComponentHeartAddStats::TYPE;
    }

    void ComponentHeartAddStats::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentHeartAddStats>(this);
    }

    void ComponentHeartAddStats::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentHeartAddStats>(this);
    }

    intrusive_ptr<ComponentBase> ComponentHeartAddStats::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentHeartAddStats>(id);
    }

    void ComponentHeartAddStats::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(percent, "percent", float(1));
        serializer.serialize(stat_upgrade, "stat_upgrade");
    }

    void ComponentHeartAddStats::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(percent, "percent", float(1));
        deserializer.deserialize(stat_upgrade, "stat_upgrade");
    }

    void ComponentHeartAddStats::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(percent, "percent", float(1));
        serializer.serialize(stat_upgrade, "stat_upgrade");
    }

    void ComponentHeartAddStats::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(percent, "percent", float(1));
        deserializer.deserialize(stat_upgrade, "stat_upgrade");
    }

} //namespace mg
