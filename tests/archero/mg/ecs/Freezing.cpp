#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Freezing.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Freezing::TYPE("Freezing");

    Freezing::Freezing()
    : activated(false)
    , life_time(0)
    , duration(0)
    , damage(0)
    {

    }

    Freezing::~Freezing()
    {
    }

    bool Freezing::operator ==(const Freezing& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->activated == rhs.activated;
        result = result && this->life_time == rhs.life_time;
        result = result && this->duration == rhs.duration;
        result = result && this->damage == rhs.damage;
        return result;
    }

    bool Freezing::operator !=(const Freezing& rhs) const
    {
        return !(*this == rhs);
    }

    Freezing::Freezing(const Freezing& rhs)
    {
        this->operator=(rhs);
    }

    const Freezing& Freezing::operator =(const Freezing& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->activated = rhs.activated;
        this->life_time = rhs.life_time;
        this->duration = rhs.duration;
        this->damage = rhs.damage;
        return *this;
    }

    std::string Freezing::get_type() const
    {
        return Freezing::TYPE;
    }

    void Freezing::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<Freezing>(this);
    }

    void Freezing::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<Freezing>(this);
    }

    intrusive_ptr<ComponentBase> Freezing::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<Freezing>(id);
    }

    void Freezing::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(activated, "activated", bool(false));
        serializer.serialize(life_time, "life_time", float(0));
        serializer.serialize(duration, "duration", float(0));
        serializer.serialize(damage, "damage", float(0));
    }

    void Freezing::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(activated, "activated", bool(false));
        deserializer.deserialize(life_time, "life_time", float(0));
        deserializer.deserialize(duration, "duration", float(0));
        deserializer.deserialize(damage, "damage", float(0));
    }

    void Freezing::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(activated, "activated", bool(false));
        serializer.serialize(life_time, "life_time", float(0));
        serializer.serialize(duration, "duration", float(0));
        serializer.serialize(damage, "damage", float(0));
    }

    void Freezing::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(activated, "activated", bool(false));
        deserializer.deserialize(life_time, "life_time", float(0));
        deserializer.deserialize(duration, "duration", float(0));
        deserializer.deserialize(damage, "damage", float(0));
    }

} //namespace mg
