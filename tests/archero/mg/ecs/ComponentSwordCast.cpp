#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentSwordCast.h"
#include "ModelEcsBase.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSwordCast::TYPE("ComponentSwordCast");

    ComponentSwordCast::ComponentSwordCast()
    : wave_event_listen(false)
    , points()
    , timer_1(0.0)
    , timer_10(0.0)
    {

    }

    ComponentSwordCast::~ComponentSwordCast()
    {
    }

    bool ComponentSwordCast::operator ==(const ComponentSwordCast& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->wave_event_listen == rhs.wave_event_listen;
        result = result && this->points == rhs.points;
        result = result && this->timer_1 == rhs.timer_1;
        result = result && this->timer_10 == rhs.timer_10;
        return result;
    }

    bool ComponentSwordCast::operator !=(const ComponentSwordCast& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSwordCast::ComponentSwordCast(const ComponentSwordCast& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSwordCast& ComponentSwordCast::operator =(const ComponentSwordCast& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->wave_event_listen = rhs.wave_event_listen;
        this->points = rhs.points;
        this->timer_1 = rhs.timer_1;
        this->timer_10 = rhs.timer_10;
        return *this;
    }

    std::string ComponentSwordCast::get_type() const
    {
        return ComponentSwordCast::TYPE;
    }

    void ComponentSwordCast::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSwordCast>(this);
    }

    void ComponentSwordCast::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSwordCast>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSwordCast::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSwordCast>(id);
    }

    void ComponentSwordCast::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(wave_event_listen, "wave_event_listen", bool(false));
        serializer.serialize(points, "points");
        serializer.serialize(timer_1, "timer_1", float(0.0));
        serializer.serialize(timer_10, "timer_10", float(0.0));
    }

    void ComponentSwordCast::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(wave_event_listen, "wave_event_listen", bool(false));
        deserializer.deserialize(points, "points");
        deserializer.deserialize(timer_1, "timer_1", float(0.0));
        deserializer.deserialize(timer_10, "timer_10", float(0.0));
    }

    void ComponentSwordCast::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(wave_event_listen, "wave_event_listen", bool(false));
        serializer.serialize(points, "points");
        serializer.serialize(timer_1, "timer_1", float(0.0));
        serializer.serialize(timer_10, "timer_10", float(0.0));
    }

    void ComponentSwordCast::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(wave_event_listen, "wave_event_listen", bool(false));
        deserializer.deserialize(points, "points");
        deserializer.deserialize(timer_1, "timer_1", float(0.0));
        deserializer.deserialize(timer_10, "timer_10", float(0.0));
    }

} //namespace mg
