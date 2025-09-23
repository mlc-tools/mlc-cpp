#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "FireDamage.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string FireDamage::TYPE("FireDamage");

    FireDamage::FireDamage()
    : value(0)
    , burning_timer(0)
    , burning(false)
    , shooter_id(0)
    , damage_timer(0)
    {

    }

    FireDamage::~FireDamage()
    {
    }

    bool FireDamage::operator ==(const FireDamage& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->value == rhs.value;
        result = result && this->burning_timer == rhs.burning_timer;
        result = result && this->burning == rhs.burning;
        result = result && this->shooter_id == rhs.shooter_id;
        result = result && this->damage_timer == rhs.damage_timer;
        return result;
    }

    bool FireDamage::operator !=(const FireDamage& rhs) const
    {
        return !(*this == rhs);
    }

    FireDamage::FireDamage(const FireDamage& rhs)
    {
        this->operator=(rhs);
    }

    const FireDamage& FireDamage::operator =(const FireDamage& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->value = rhs.value;
        this->burning_timer = rhs.burning_timer;
        this->burning = rhs.burning;
        this->shooter_id = rhs.shooter_id;
        this->damage_timer = rhs.damage_timer;
        return *this;
    }

    std::string FireDamage::get_type() const
    {
        return FireDamage::TYPE;
    }

    void FireDamage::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<FireDamage>(this);
    }

    void FireDamage::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<FireDamage>(this);
    }

    intrusive_ptr<ComponentBase> FireDamage::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<FireDamage>(id);
    }

    void FireDamage::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(value, "value", float(0));
        serializer.serialize(burning_timer, "burning_timer", float(0));
        serializer.serialize(burning, "burning", bool(false));
        serializer.serialize(shooter_id, "shooter_id", int(0));
        serializer.serialize(damage_timer, "damage_timer", float(0));
    }

    void FireDamage::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(value, "value", float(0));
        deserializer.deserialize(burning_timer, "burning_timer", float(0));
        deserializer.deserialize(burning, "burning", bool(false));
        deserializer.deserialize(shooter_id, "shooter_id", int(0));
        deserializer.deserialize(damage_timer, "damage_timer", float(0));
    }

    void FireDamage::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(value, "value", float(0));
        serializer.serialize(burning_timer, "burning_timer", float(0));
        serializer.serialize(burning, "burning", bool(false));
        serializer.serialize(shooter_id, "shooter_id", int(0));
        serializer.serialize(damage_timer, "damage_timer", float(0));
    }

    void FireDamage::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(value, "value", float(0));
        deserializer.deserialize(burning_timer, "burning_timer", float(0));
        deserializer.deserialize(burning, "burning", bool(false));
        deserializer.deserialize(shooter_id, "shooter_id", int(0));
        deserializer.deserialize(damage_timer, "damage_timer", float(0));
    }

} //namespace mg
