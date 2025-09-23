#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentMeteorPeriodic.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentMeteorPeriodic::TYPE("ComponentMeteorPeriodic");

    ComponentMeteorPeriodic::ComponentMeteorPeriodic()
    : period(3)
    , timer(0)
    {

    }

    ComponentMeteorPeriodic::~ComponentMeteorPeriodic()
    {
    }

    bool ComponentMeteorPeriodic::operator ==(const ComponentMeteorPeriodic& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->period == rhs.period;
        result = result && this->timer == rhs.timer;
        return result;
    }

    bool ComponentMeteorPeriodic::operator !=(const ComponentMeteorPeriodic& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentMeteorPeriodic::ComponentMeteorPeriodic(const ComponentMeteorPeriodic& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentMeteorPeriodic& ComponentMeteorPeriodic::operator =(const ComponentMeteorPeriodic& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->period = rhs.period;
        this->timer = rhs.timer;
        return *this;
    }

    std::string ComponentMeteorPeriodic::get_type() const
    {
        return ComponentMeteorPeriodic::TYPE;
    }

    void ComponentMeteorPeriodic::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentMeteorPeriodic>(this);
    }

    void ComponentMeteorPeriodic::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentMeteorPeriodic>(this);
    }

    intrusive_ptr<ComponentBase> ComponentMeteorPeriodic::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentMeteorPeriodic>(id);
    }

    void ComponentMeteorPeriodic::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(period, "period", float(3));
        serializer.serialize(timer, "timer", float(0));
    }

    void ComponentMeteorPeriodic::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(period, "period", float(3));
        deserializer.deserialize(timer, "timer", float(0));
    }

    void ComponentMeteorPeriodic::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(period, "period", float(3));
        serializer.serialize(timer, "timer", float(0));
    }

    void ComponentMeteorPeriodic::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(period, "period", float(3));
        deserializer.deserialize(timer, "timer", float(0));
    }

} //namespace mg
