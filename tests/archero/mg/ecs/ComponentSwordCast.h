#ifndef __mg_ComponentSwordCast_h__
#define __mg_ComponentSwordCast_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "Vector.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;
    class Vector;

    class ComponentSwordCast : public ComponentBase
    {
    public:
        ComponentSwordCast();
        virtual ~ComponentSwordCast();
        bool operator ==(const ComponentSwordCast& rhs) const;
        bool operator !=(const ComponentSwordCast& rhs) const;
        ComponentSwordCast(const ComponentSwordCast& rhs);
        const ComponentSwordCast& operator =(const ComponentSwordCast& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        bool wave_event_listen;
        std::vector<Vector> points;
        float timer_1;
        float timer_10;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSwordCast_h__
