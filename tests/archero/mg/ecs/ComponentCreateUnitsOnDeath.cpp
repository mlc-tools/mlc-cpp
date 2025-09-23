#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentCreateUnitsOnDeath.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentCreateUnitsOnDeath::TYPE("ComponentCreateUnitsOnDeath");

    ComponentCreateUnitsOnDeath::ComponentCreateUnitsOnDeath()
    : units()
    , radius(100)
    {

    }

    ComponentCreateUnitsOnDeath::~ComponentCreateUnitsOnDeath()
    {
    }

    bool ComponentCreateUnitsOnDeath::operator ==(const ComponentCreateUnitsOnDeath& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->units == rhs.units;
        result = result && this->radius == rhs.radius;
        return result;
    }

    bool ComponentCreateUnitsOnDeath::operator !=(const ComponentCreateUnitsOnDeath& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentCreateUnitsOnDeath::ComponentCreateUnitsOnDeath(const ComponentCreateUnitsOnDeath& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentCreateUnitsOnDeath& ComponentCreateUnitsOnDeath::operator =(const ComponentCreateUnitsOnDeath& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->units = rhs.units;
        this->radius = rhs.radius;
        return *this;
    }

    std::string ComponentCreateUnitsOnDeath::get_type() const
    {
        return ComponentCreateUnitsOnDeath::TYPE;
    }

    void ComponentCreateUnitsOnDeath::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentCreateUnitsOnDeath>(this);
    }

    void ComponentCreateUnitsOnDeath::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentCreateUnitsOnDeath>(this);
    }

    intrusive_ptr<ComponentBase> ComponentCreateUnitsOnDeath::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentCreateUnitsOnDeath>(id);
    }

    void ComponentCreateUnitsOnDeath::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(radius, "radius", float(100));
    }

    void ComponentCreateUnitsOnDeath::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(radius, "radius", float(100));
    }

    void ComponentCreateUnitsOnDeath::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(radius, "radius", float(100));
    }

    void ComponentCreateUnitsOnDeath::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(radius, "radius", float(100));
    }

} //namespace mg
