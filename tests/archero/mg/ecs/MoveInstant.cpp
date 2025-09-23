#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentMovement.h"
#include "ModelEcsBase.h"
#include "MoveInstant.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string MoveInstant::TYPE("MoveInstant");

    MoveInstant::MoveInstant()
    : target_id(0)
    {

    }

    MoveInstant::~MoveInstant()
    {
    }

    bool MoveInstant::operator ==(const MoveInstant& rhs) const
    {
        bool result = this->ComponentMovement::operator ==(rhs);
        result = result && this->target_id == rhs.target_id;
        return result;
    }

    bool MoveInstant::operator !=(const MoveInstant& rhs) const
    {
        return !(*this == rhs);
    }

    MoveInstant::MoveInstant(const MoveInstant& rhs)
    {
        this->operator=(rhs);
    }

    const MoveInstant& MoveInstant::operator =(const MoveInstant& rhs)
    {
        this->ComponentMovement::operator=(rhs);
        this->target_id = rhs.target_id;
        return *this;
    }

    std::string MoveInstant::get_type() const
    {
        return MoveInstant::TYPE;
    }

    void MoveInstant::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<MoveInstant>(this);
    }

    void MoveInstant::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<MoveInstant>(this);
    }

    intrusive_ptr<ComponentBase> MoveInstant::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<MoveInstant>(id);
    }

    void MoveInstant::serialize_xml(SerializerXml& serializer) const
    {
        ComponentMovement::serialize_xml(serializer);
        serializer.serialize(target_id, "target_id", int(0));
    }

    void MoveInstant::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentMovement::deserialize_xml(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
    }

    void MoveInstant::serialize_json(SerializerJson& serializer) const
    {
        ComponentMovement::serialize_json(serializer);
        serializer.serialize(target_id, "target_id", int(0));
    }

    void MoveInstant::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentMovement::deserialize_json(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
    }

} //namespace mg
