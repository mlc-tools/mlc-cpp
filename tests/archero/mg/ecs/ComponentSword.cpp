#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentSword.h"
#include "ComponentSwordState.h"
#include "ModelEcsBase.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSword::TYPE("ComponentSword");

    ComponentSword::ComponentSword()
    : state(ComponentSwordState::appearance)
    , target_id(0)
    , target_pos()
    , player_offset()
    , timer(0.5)
    , appearance_duration(0.4)
    , prepare_to_fly_duration(0.5)
    , fly_duration(0)
    , fly_speed(1000)
    , clean(false)
    , prepare_fly_start_position()
    , fly_start_position()
    , pos_destination()
    {

    }

    ComponentSword::~ComponentSword()
    {
    }

    bool ComponentSword::operator ==(const ComponentSword& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->state == rhs.state;
        result = result && this->target_id == rhs.target_id;
        result = result && this->target_pos == rhs.target_pos;
        result = result && this->player_offset == rhs.player_offset;
        result = result && this->timer == rhs.timer;
        result = result && this->appearance_duration == rhs.appearance_duration;
        result = result && this->prepare_to_fly_duration == rhs.prepare_to_fly_duration;
        result = result && this->fly_duration == rhs.fly_duration;
        result = result && this->fly_speed == rhs.fly_speed;
        result = result && this->clean == rhs.clean;
        result = result && this->prepare_fly_start_position == rhs.prepare_fly_start_position;
        result = result && this->fly_start_position == rhs.fly_start_position;
        result = result && this->pos_destination == rhs.pos_destination;
        return result;
    }

    bool ComponentSword::operator !=(const ComponentSword& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSword::ComponentSword(const ComponentSword& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSword& ComponentSword::operator =(const ComponentSword& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->state = rhs.state;
        this->target_id = rhs.target_id;
        this->target_pos = rhs.target_pos;
        this->player_offset = rhs.player_offset;
        this->timer = rhs.timer;
        this->appearance_duration = rhs.appearance_duration;
        this->prepare_to_fly_duration = rhs.prepare_to_fly_duration;
        this->fly_duration = rhs.fly_duration;
        this->fly_speed = rhs.fly_speed;
        this->clean = rhs.clean;
        this->prepare_fly_start_position = rhs.prepare_fly_start_position;
        this->fly_start_position = rhs.fly_start_position;
        this->pos_destination = rhs.pos_destination;
        return *this;
    }

    std::string ComponentSword::get_type() const
    {
        return ComponentSword::TYPE;
    }

    void ComponentSword::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSword>(this);
    }

    void ComponentSword::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSword>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSword::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSword>(id);
    }

    void ComponentSword::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(state, "state");
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(target_pos, "target_pos");
        serializer.serialize(player_offset, "player_offset");
        serializer.serialize(timer, "timer", float(0.5));
        serializer.serialize(appearance_duration, "appearance_duration", float(0.4));
        serializer.serialize(prepare_to_fly_duration, "prepare_to_fly_duration", float(0.5));
        serializer.serialize(fly_duration, "fly_duration", float(0));
        serializer.serialize(fly_speed, "fly_speed", float(1000));
        serializer.serialize(clean, "clean", bool(false));
        serializer.serialize(prepare_fly_start_position, "prepare_fly_start_position");
        serializer.serialize(fly_start_position, "fly_start_position");
        serializer.serialize(pos_destination, "pos_destination");
    }

    void ComponentSword::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(state, "state");
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(target_pos, "target_pos");
        deserializer.deserialize(player_offset, "player_offset");
        deserializer.deserialize(timer, "timer", float(0.5));
        deserializer.deserialize(appearance_duration, "appearance_duration", float(0.4));
        deserializer.deserialize(prepare_to_fly_duration, "prepare_to_fly_duration", float(0.5));
        deserializer.deserialize(fly_duration, "fly_duration", float(0));
        deserializer.deserialize(fly_speed, "fly_speed", float(1000));
        deserializer.deserialize(clean, "clean", bool(false));
        deserializer.deserialize(prepare_fly_start_position, "prepare_fly_start_position");
        deserializer.deserialize(fly_start_position, "fly_start_position");
        deserializer.deserialize(pos_destination, "pos_destination");
    }

    void ComponentSword::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(state, "state");
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(target_pos, "target_pos");
        serializer.serialize(player_offset, "player_offset");
        serializer.serialize(timer, "timer", float(0.5));
        serializer.serialize(appearance_duration, "appearance_duration", float(0.4));
        serializer.serialize(prepare_to_fly_duration, "prepare_to_fly_duration", float(0.5));
        serializer.serialize(fly_duration, "fly_duration", float(0));
        serializer.serialize(fly_speed, "fly_speed", float(1000));
        serializer.serialize(clean, "clean", bool(false));
        serializer.serialize(prepare_fly_start_position, "prepare_fly_start_position");
        serializer.serialize(fly_start_position, "fly_start_position");
        serializer.serialize(pos_destination, "pos_destination");
    }

    void ComponentSword::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(state, "state");
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(target_pos, "target_pos");
        deserializer.deserialize(player_offset, "player_offset");
        deserializer.deserialize(timer, "timer", float(0.5));
        deserializer.deserialize(appearance_duration, "appearance_duration", float(0.4));
        deserializer.deserialize(prepare_to_fly_duration, "prepare_to_fly_duration", float(0.5));
        deserializer.deserialize(fly_duration, "fly_duration", float(0));
        deserializer.deserialize(fly_speed, "fly_speed", float(1000));
        deserializer.deserialize(clean, "clean", bool(false));
        deserializer.deserialize(prepare_fly_start_position, "prepare_fly_start_position");
        deserializer.deserialize(fly_start_position, "fly_start_position");
        deserializer.deserialize(pos_destination, "pos_destination");
    }

} //namespace mg
