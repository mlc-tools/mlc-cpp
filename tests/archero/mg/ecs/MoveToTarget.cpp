#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ModelEcsBase.h"
#include "MoveToTarget.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string MoveToTarget::TYPE("MoveToTarget");

    MoveToTarget::MoveToTarget()
    : target_id(0)
    , offset()
    , changed_direction()
    , timer_to_move_by_changed_direction(0.0)
    {

    }

    MoveToTarget::~MoveToTarget()
    {
    }

    bool MoveToTarget::operator ==(const MoveToTarget& rhs) const
    {
        bool result = this->ComponentMovement::operator ==(rhs);
        result = result && this->target_id == rhs.target_id;
        result = result && this->offset == rhs.offset;
        result = result && this->changed_direction == rhs.changed_direction;
        result = result && this->timer_to_move_by_changed_direction == rhs.timer_to_move_by_changed_direction;
        return result;
    }

    bool MoveToTarget::operator !=(const MoveToTarget& rhs) const
    {
        return !(*this == rhs);
    }

    MoveToTarget::MoveToTarget(const MoveToTarget& rhs)
    {
        this->operator=(rhs);
    }

    const MoveToTarget& MoveToTarget::operator =(const MoveToTarget& rhs)
    {
        this->ComponentMovement::operator=(rhs);
        this->target_id = rhs.target_id;
        this->offset = rhs.offset;
        this->changed_direction = rhs.changed_direction;
        this->timer_to_move_by_changed_direction = rhs.timer_to_move_by_changed_direction;
        return *this;
    }

    std::string MoveToTarget::get_type() const
    {
        return MoveToTarget::TYPE;
    }

    void MoveToTarget::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<MoveToTarget>(this);
    }

    void MoveToTarget::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<MoveToTarget>(this);
    }

    intrusive_ptr<ComponentBase> MoveToTarget::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<MoveToTarget>(id);
    }

    void MoveToTarget::serialize_xml(SerializerXml& serializer) const
    {
        ComponentMovement::serialize_xml(serializer);
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(offset, "offset");
        serializer.serialize(changed_direction, "changed_direction");
        serializer.serialize(timer_to_move_by_changed_direction, "timer_to_move_by_changed_direction", float(0.0));
    }

    void MoveToTarget::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentMovement::deserialize_xml(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(offset, "offset");
        deserializer.deserialize(changed_direction, "changed_direction");
        deserializer.deserialize(timer_to_move_by_changed_direction, "timer_to_move_by_changed_direction", float(0.0));
    }

    void MoveToTarget::serialize_json(SerializerJson& serializer) const
    {
        ComponentMovement::serialize_json(serializer);
        serializer.serialize(target_id, "target_id", int(0));
        serializer.serialize(offset, "offset");
        serializer.serialize(changed_direction, "changed_direction");
        serializer.serialize(timer_to_move_by_changed_direction, "timer_to_move_by_changed_direction", float(0.0));
    }

    void MoveToTarget::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentMovement::deserialize_json(deserializer);
        deserializer.deserialize(target_id, "target_id", int(0));
        deserializer.deserialize(offset, "offset");
        deserializer.deserialize(changed_direction, "changed_direction");
        deserializer.deserialize(timer_to_move_by_changed_direction, "timer_to_move_by_changed_direction", float(0.0));
    }

} //namespace mg
