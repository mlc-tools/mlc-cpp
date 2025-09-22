#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ElectricDamage.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ElectricDamage::TYPE("ElectricDamage");

    ElectricDamage::ElectricDamage()
    : value(0)
    , shooter_id(0)
    {

    }

    ElectricDamage::~ElectricDamage()
    {
    }

    bool ElectricDamage::operator ==(const ElectricDamage& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->value == rhs.value;
        result = result && this->shooter_id == rhs.shooter_id;
        return result;
    }

    bool ElectricDamage::operator !=(const ElectricDamage& rhs) const
    {
        return !(*this == rhs);
    }

    ElectricDamage::ElectricDamage(const ElectricDamage& rhs)
    {
        this->operator=(rhs);
    }

    const ElectricDamage& ElectricDamage::operator =(const ElectricDamage& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->value = rhs.value;
        this->shooter_id = rhs.shooter_id;
        return *this;
    }

    std::string ElectricDamage::get_type() const
    {
        return ElectricDamage::TYPE;
    }

    void ElectricDamage::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ElectricDamage>(this);
    }

    void ElectricDamage::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ElectricDamage>(this);
    }

    intrusive_ptr<ComponentBase> ElectricDamage::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ElectricDamage>(id);
    }

    void ElectricDamage::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(value, "value", float(0));
        serializer.serialize(shooter_id, "shooter_id", int(0));
    }

    void ElectricDamage::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(value, "value", float(0));
        deserializer.deserialize(shooter_id, "shooter_id", int(0));
    }

    void ElectricDamage::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(value, "value", float(0));
        serializer.serialize(shooter_id, "shooter_id", int(0));
    }

    void ElectricDamage::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(value, "value", float(0));
        deserializer.deserialize(shooter_id, "shooter_id", int(0));
    }

} //namespace mg
