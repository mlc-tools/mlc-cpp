#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ModelEcsBase.h"
#include "MoveParabolic.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string MoveParabolic::TYPE("MoveParabolic");

    MoveParabolic::MoveParabolic()
    : start_position()
    , target_position()
    , offset()
    , height(0.0f)
    , timer(0.0f)
    , duration(0.0f)
    , done(false)
    , z(0.0f)
    {

    }

    MoveParabolic::~MoveParabolic()
    {
    }

    float MoveParabolic::get_duration() const
    {
        return this->duration;
    }

    float MoveParabolic::get_height() const
    {
        return this->height;
    }

    void MoveParabolic::set(const Vector& start_position, const Vector& target_position, float speed)
    {
        this->start_position = start_position;
        this->target_position = target_position;
        this->offset = target_position - start_position;
        auto dist = offset.length();
        this->height = dist / 3;
        this->duration = dist / speed;
    }

    Vector MoveParabolic::update_position(float dt)
    {
        this->timer = std::min(this->timer + dt, this->duration);
        auto t = this->timer / this->duration;
        if(t >= 1)
        {
            t = 1;
            this->done = true;
        }
        auto result = start_position + offset * t;

        this->z = sin(M_PI * t) * this->height;

        return result;
    }

    bool MoveParabolic::operator ==(const MoveParabolic& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->start_position == rhs.start_position;
        result = result && this->target_position == rhs.target_position;
        result = result && this->offset == rhs.offset;
        result = result && this->height == rhs.height;
        result = result && this->timer == rhs.timer;
        result = result && this->duration == rhs.duration;
        result = result && this->done == rhs.done;
        result = result && this->z == rhs.z;
        return result;
    }

    bool MoveParabolic::operator !=(const MoveParabolic& rhs) const
    {
        return !(*this == rhs);
    }

    MoveParabolic::MoveParabolic(const MoveParabolic& rhs)
    {
        this->operator=(rhs);
    }

    const MoveParabolic& MoveParabolic::operator =(const MoveParabolic& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->start_position = rhs.start_position;
        this->target_position = rhs.target_position;
        this->offset = rhs.offset;
        this->height = rhs.height;
        this->timer = rhs.timer;
        this->duration = rhs.duration;
        this->done = rhs.done;
        this->z = rhs.z;
        return *this;
    }

    std::string MoveParabolic::get_type() const
    {
        return MoveParabolic::TYPE;
    }

    void MoveParabolic::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<MoveParabolic>(this);
    }

    void MoveParabolic::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<MoveParabolic>(this);
    }

    intrusive_ptr<ComponentBase> MoveParabolic::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<MoveParabolic>(id);
    }

    void MoveParabolic::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(start_position, "start_position");
        serializer.serialize(target_position, "target_position");
        serializer.serialize(offset, "offset");
        serializer.serialize(height, "height", float(0.0f));
        serializer.serialize(timer, "timer", float(0.0f));
        serializer.serialize(duration, "duration", float(0.0f));
        serializer.serialize(done, "done", bool(false));
        serializer.serialize(z, "z", float(0.0f));
    }

    void MoveParabolic::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(start_position, "start_position");
        deserializer.deserialize(target_position, "target_position");
        deserializer.deserialize(offset, "offset");
        deserializer.deserialize(height, "height", float(0.0f));
        deserializer.deserialize(timer, "timer", float(0.0f));
        deserializer.deserialize(duration, "duration", float(0.0f));
        deserializer.deserialize(done, "done", bool(false));
        deserializer.deserialize(z, "z", float(0.0f));
    }

    void MoveParabolic::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(start_position, "start_position");
        serializer.serialize(target_position, "target_position");
        serializer.serialize(offset, "offset");
        serializer.serialize(height, "height", float(0.0f));
        serializer.serialize(timer, "timer", float(0.0f));
        serializer.serialize(duration, "duration", float(0.0f));
        serializer.serialize(done, "done", bool(false));
        serializer.serialize(z, "z", float(0.0f));
    }

    void MoveParabolic::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(start_position, "start_position");
        deserializer.deserialize(target_position, "target_position");
        deserializer.deserialize(offset, "offset");
        deserializer.deserialize(height, "height", float(0.0f));
        deserializer.deserialize(timer, "timer", float(0.0f));
        deserializer.deserialize(duration, "duration", float(0.0f));
        deserializer.deserialize(done, "done", bool(false));
        deserializer.deserialize(z, "z", float(0.0f));
    }

} //namespace mg
