#ifndef __mg_MoveVertical_h__
#define __mg_MoveVertical_h__

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

    class MoveVertical : public ComponentMovement
    {
    public:
        MoveVertical();
        virtual ~MoveVertical();
        bool operator ==(const MoveVertical& rhs) const;
        bool operator !=(const MoveVertical& rhs) const;
        MoveVertical(const MoveVertical& rhs);
        const MoveVertical& operator =(const MoveVertical& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id) override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        Vector destination;
        float start_z;
        float duration;
        float timer;
        bool done;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_MoveVertical_h__
