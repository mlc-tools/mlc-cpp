#ifndef __mg_ComponentAuraDamage_h__
#define __mg_ComponentAuraDamage_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentAura.h"
#include "ComponentBase.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentAuraDamage : public ComponentAura
    {
    public:
        ComponentAuraDamage();
        virtual ~ComponentAuraDamage();
        bool operator ==(const ComponentAuraDamage& rhs) const;
        bool operator !=(const ComponentAuraDamage& rhs) const;
        ComponentAuraDamage(const ComponentAuraDamage& rhs);
        const ComponentAuraDamage& operator =(const ComponentAuraDamage& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id) override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        float damage_mul;
        std::map<int, float> damaged_entities;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentAuraDamage_h__
