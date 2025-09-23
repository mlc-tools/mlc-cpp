#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentMovement.h"
#include "ModelEcsBase.h"
#include "MoveDirection.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string MoveDirection::TYPE("MoveDirection");

    MoveDirection::MoveDirection()
    : clean(false)
    {

    }

    MoveDirection::~MoveDirection()
    {
    }

    bool MoveDirection::operator ==(const MoveDirection& rhs) const
    {
        bool result = this->ComponentMovement::operator ==(rhs);
        result = result && this->clean == rhs.clean;
        return result;
    }

    bool MoveDirection::operator !=(const MoveDirection& rhs) const
    {
        return !(*this == rhs);
    }

    MoveDirection::MoveDirection(const MoveDirection& rhs)
    {
        this->operator=(rhs);
    }

    const MoveDirection& MoveDirection::operator =(const MoveDirection& rhs)
    {
        this->ComponentMovement::operator=(rhs);
        this->clean = rhs.clean;
        return *this;
    }

    std::string MoveDirection::get_type() const
    {
        return MoveDirection::TYPE;
    }

    void MoveDirection::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<MoveDirection>(this);
    }

    void MoveDirection::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<MoveDirection>(this);
    }

    intrusive_ptr<ComponentBase> MoveDirection::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<MoveDirection>(id);
    }

    void MoveDirection::serialize_xml(SerializerXml& serializer) const
    {
        ComponentMovement::serialize_xml(serializer);
        serializer.serialize(clean, "clean", bool(false));
    }

    void MoveDirection::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentMovement::deserialize_xml(deserializer);
        deserializer.deserialize(clean, "clean", bool(false));
    }

    void MoveDirection::serialize_json(SerializerJson& serializer) const
    {
        ComponentMovement::serialize_json(serializer);
        serializer.serialize(clean, "clean", bool(false));
    }

    void MoveDirection::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentMovement::deserialize_json(deserializer);
        deserializer.deserialize(clean, "clean", bool(false));
    }

} //namespace mg
