#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentStun.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentStun::TYPE("ComponentStun");

    ComponentStun::ComponentStun()
    : activated(false)
    , timer(0)
    , resist_timer(0)
    {

    }

    ComponentStun::~ComponentStun()
    {
    }

    bool ComponentStun::operator ==(const ComponentStun& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->activated == rhs.activated;
        result = result && this->timer == rhs.timer;
        result = result && this->resist_timer == rhs.resist_timer;
        return result;
    }

    bool ComponentStun::operator !=(const ComponentStun& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentStun::ComponentStun(const ComponentStun& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentStun& ComponentStun::operator =(const ComponentStun& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->activated = rhs.activated;
        this->timer = rhs.timer;
        this->resist_timer = rhs.resist_timer;
        return *this;
    }

    std::string ComponentStun::get_type() const
    {
        return ComponentStun::TYPE;
    }

    void ComponentStun::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentStun>(this);
    }

    void ComponentStun::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentStun>(this);
    }

    intrusive_ptr<ComponentBase> ComponentStun::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentStun>(id);
    }

    void ComponentStun::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(activated, "activated", bool(false));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(resist_timer, "resist_timer", float(0));
    }

    void ComponentStun::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(activated, "activated", bool(false));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(resist_timer, "resist_timer", float(0));
    }

    void ComponentStun::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(activated, "activated", bool(false));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(resist_timer, "resist_timer", float(0));
    }

    void ComponentStun::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(activated, "activated", bool(false));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(resist_timer, "resist_timer", float(0));
    }

} //namespace mg
