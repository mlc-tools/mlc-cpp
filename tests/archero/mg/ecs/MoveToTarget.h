#ifndef __mg_MoveToTarget_h__
#define __mg_MoveToTarget_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "ComponentMovement.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class MoveToTarget : public ComponentMovement
    {
    public:
        MoveToTarget();
        virtual ~MoveToTarget();
        bool operator ==(const MoveToTarget& rhs) const;
        bool operator !=(const MoveToTarget& rhs) const;
        MoveToTarget(const MoveToTarget& rhs);
        const MoveToTarget& operator =(const MoveToTarget& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id) override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int target_id;
        Vector offset;
        Vector changed_direction;
        float timer_to_move_by_changed_direction;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_MoveToTarget_h__
