#ifndef __mg_ComponentDamage_h__
#define __mg_ComponentDamage_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentDamage : public ComponentBase
    {
    public:
        ComponentDamage();
        virtual ~ComponentDamage();
        bool operator ==(const ComponentDamage& rhs) const;
        bool operator !=(const ComponentDamage& rhs) const;
        ComponentDamage(const ComponentDamage& rhs);
        const ComponentDamage& operator =(const ComponentDamage& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int attacker_id;
        int bullet_id;
        int target_id;
        float damage;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentDamage_h__
