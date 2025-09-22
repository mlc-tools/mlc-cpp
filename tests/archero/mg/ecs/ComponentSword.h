#ifndef __mg_ComponentSword_h__
#define __mg_ComponentSword_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "ComponentSwordState.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentSword : public ComponentBase
    {
    public:
        ComponentSword();
        virtual ~ComponentSword();
        bool operator ==(const ComponentSword& rhs) const;
        bool operator !=(const ComponentSword& rhs) const;
        ComponentSword(const ComponentSword& rhs);
        const ComponentSword& operator =(const ComponentSword& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        ComponentSwordState state;
        int target_id;
        Vector target_pos;
        Vector player_offset;
        float timer;
        float appearance_duration;
        float prepare_to_fly_duration;
        float fly_duration;
        float fly_speed;
        bool clean;
        Vector prepare_fly_start_position;
        Vector fly_start_position;
        Vector pos_destination;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSword_h__
