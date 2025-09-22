#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BuilderDamage.h"
#include "ComponentDamage.h"
#include "ModelEcsBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string BuilderDamage::TYPE("BuilderDamage");

    BuilderDamage::BuilderDamage()
    : damage(0.0)
    , _reference_counter(1)
    {

    }

    BuilderDamage::~BuilderDamage()
    {
    }

    BuilderDamage& BuilderDamage::set_damage(float damage)
    {
        this->damage = damage;
        return *this;
    }

    int BuilderDamage::build(ModelEcsBase* model, int attacker_id, int bullet_id, int target_id)
    {
        auto id = model->get_free_id();
        auto damage = make_intrusive<ComponentDamage>();
        damage->attacker_id = attacker_id;
        damage->bullet_id = bullet_id;
        damage->target_id = target_id;
        damage->damage = this->damage;
        model->add(damage, id);
        return id;
    }

    void BuilderDamage::retain()
    {
        ++this->_reference_counter;
    }

    int BuilderDamage::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool BuilderDamage::operator ==(const BuilderDamage& rhs) const
    {
        bool result = true;
        result = result && this->damage == rhs.damage;
        return result;
    }

    bool BuilderDamage::operator !=(const BuilderDamage& rhs) const
    {
        return !(*this == rhs);
    }

    BuilderDamage::BuilderDamage(const BuilderDamage& rhs)
    {
        this->operator=(rhs);
    }

    const BuilderDamage& BuilderDamage::operator =(const BuilderDamage& rhs)
    {
        this->damage = rhs.damage;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string BuilderDamage::get_type() const
    {
        return BuilderDamage::TYPE;
    }

    void BuilderDamage::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(damage, "damage", float(0.0));
    }

    void BuilderDamage::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(damage, "damage", float(0.0));
    }

    void BuilderDamage::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(damage, "damage", float(0.0));
    }

    void BuilderDamage::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(damage, "damage", float(0.0));
    }

} //namespace mg
