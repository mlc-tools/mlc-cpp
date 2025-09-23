#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentMovement.h"
#include "ModelEcsBase.h"
#include "MoveVertical.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string MoveVertical::TYPE("MoveVertical");

    MoveVertical::MoveVertical()
    : destination()
    , start_z(10)
    , duration(0.66)
    , timer(0)
    , done(false)
    {

    }

    MoveVertical::~MoveVertical()
    {
    }

    bool MoveVertical::operator ==(const MoveVertical& rhs) const
    {
        bool result = this->ComponentMovement::operator ==(rhs);
        result = result && this->destination == rhs.destination;
        result = result && this->start_z == rhs.start_z;
        result = result && this->duration == rhs.duration;
        result = result && this->timer == rhs.timer;
        result = result && this->done == rhs.done;
        return result;
    }

    bool MoveVertical::operator !=(const MoveVertical& rhs) const
    {
        return !(*this == rhs);
    }

    MoveVertical::MoveVertical(const MoveVertical& rhs)
    {
        this->operator=(rhs);
    }

    const MoveVertical& MoveVertical::operator =(const MoveVertical& rhs)
    {
        this->ComponentMovement::operator=(rhs);
        this->destination = rhs.destination;
        this->start_z = rhs.start_z;
        this->duration = rhs.duration;
        this->timer = rhs.timer;
        this->done = rhs.done;
        return *this;
    }

    std::string MoveVertical::get_type() const
    {
        return MoveVertical::TYPE;
    }

    void MoveVertical::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<MoveVertical>(this);
    }

    void MoveVertical::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<MoveVertical>(this);
    }

    intrusive_ptr<ComponentBase> MoveVertical::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<MoveVertical>(id);
    }

    void MoveVertical::serialize_xml(SerializerXml& serializer) const
    {
        ComponentMovement::serialize_xml(serializer);
        serializer.serialize(destination, "destination");
        serializer.serialize(start_z, "start_z", float(10));
        serializer.serialize(duration, "duration", float(0.66));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(done, "done", bool(false));
    }

    void MoveVertical::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentMovement::deserialize_xml(deserializer);
        deserializer.deserialize(destination, "destination");
        deserializer.deserialize(start_z, "start_z", float(10));
        deserializer.deserialize(duration, "duration", float(0.66));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(done, "done", bool(false));
    }

    void MoveVertical::serialize_json(SerializerJson& serializer) const
    {
        ComponentMovement::serialize_json(serializer);
        serializer.serialize(destination, "destination");
        serializer.serialize(start_z, "start_z", float(10));
        serializer.serialize(duration, "duration", float(0.66));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(done, "done", bool(false));
    }

    void MoveVertical::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentMovement::deserialize_json(deserializer);
        deserializer.deserialize(destination, "destination");
        deserializer.deserialize(start_z, "start_z", float(10));
        deserializer.deserialize(duration, "duration", float(0.66));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(done, "done", bool(false));
    }

} //namespace mg
