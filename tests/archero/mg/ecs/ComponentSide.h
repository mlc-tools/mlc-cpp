#ifndef __mg_ComponentSide_h__
#define __mg_ComponentSide_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "Side.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;
    class Side;

    class ComponentSide : public ComponentBase
    {
    public:
        ComponentSide();
        virtual ~ComponentSide();
        bool is_enemy(const Side& side) const;
        bool operator ==(const ComponentSide& rhs) const;
        bool operator !=(const ComponentSide& rhs) const;
        ComponentSide(const ComponentSide& rhs);
        const ComponentSide& operator =(const ComponentSide& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        Side side;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSide_h__
