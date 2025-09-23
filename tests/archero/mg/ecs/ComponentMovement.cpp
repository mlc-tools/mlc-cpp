#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentAuraOfSlow.h"
#include "ComponentBase.h"
#include "ComponentMovement.h"
#include "ModelEcsBase.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentMovement::TYPE("ComponentMovement");

    ComponentMovement::ComponentMovement()
    : direction()
    , aura_slow(nullptr)
    , slow_k(1)
    {

    }

    ComponentMovement::~ComponentMovement()
    {
    }

    bool ComponentMovement::operator ==(const ComponentMovement& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->direction == rhs.direction;
        result = result && ((this->aura_slow == rhs.aura_slow) || (this->aura_slow != nullptr && rhs.aura_slow != nullptr && *this->aura_slow == *rhs.aura_slow));
        result = result && this->slow_k == rhs.slow_k;
        return result;
    }

    bool ComponentMovement::operator !=(const ComponentMovement& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentMovement::ComponentMovement(const ComponentMovement& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentMovement& ComponentMovement::operator =(const ComponentMovement& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->direction = rhs.direction;
        this->aura_slow = rhs.aura_slow;
        this->slow_k = rhs.slow_k;
        return *this;
    }

    std::string ComponentMovement::get_type() const
    {
        return ComponentMovement::TYPE;
    }

    void ComponentMovement::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentMovement>(this);
    }

    void ComponentMovement::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentMovement>(this);
    }

    intrusive_ptr<ComponentBase> ComponentMovement::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentMovement>(id);
    }

    void ComponentMovement::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(direction, "direction");
        serializer.serialize(aura_slow, "aura_slow");
        serializer.serialize(slow_k, "slow_k", float(1));
    }

    void ComponentMovement::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(direction, "direction");
        deserializer.deserialize(aura_slow, "aura_slow");
        deserializer.deserialize(slow_k, "slow_k", float(1));
    }

    void ComponentMovement::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(direction, "direction");
        serializer.serialize(aura_slow, "aura_slow");
        serializer.serialize(slow_k, "slow_k", float(1));
    }

    void ComponentMovement::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(direction, "direction");
        deserializer.deserialize(aura_slow, "aura_slow");
        deserializer.deserialize(slow_k, "slow_k", float(1));
    }

} //namespace mg
