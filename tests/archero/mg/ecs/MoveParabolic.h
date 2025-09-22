#ifndef __mg_MoveParabolic_h__
#define __mg_MoveParabolic_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;
    class Vector;

    class MoveParabolic : public ComponentBase
    {
    public:
        MoveParabolic();
        virtual ~MoveParabolic();
        float get_duration() const;
        float get_height() const;
        void set(const Vector& start_position, const Vector& target_position, float speed);
        Vector update_position(float dt);
        bool operator ==(const MoveParabolic& rhs) const;
        bool operator !=(const MoveParabolic& rhs) const;
        MoveParabolic(const MoveParabolic& rhs);
        const MoveParabolic& operator =(const MoveParabolic& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

    private:
        Vector start_position;
        Vector target_position;
        Vector offset;
        float height;
        float timer;
        float duration;
    public:
        bool done;
        float z;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_MoveParabolic_h__
