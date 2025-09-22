#ifndef __mg_ComponentMovement_h__
#define __mg_ComponentMovement_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentAuraOfSlow.h"
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

    class ComponentMovement : public ComponentBase
    {
    public:
        ComponentMovement();
        virtual ~ComponentMovement();
        bool operator ==(const ComponentMovement& rhs) const;
        bool operator !=(const ComponentMovement& rhs) const;
        ComponentMovement(const ComponentMovement& rhs);
        const ComponentMovement& operator =(const ComponentMovement& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        Vector direction;
        intrusive_ptr<ComponentAuraOfSlow> aura_slow;
        float slow_k;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentMovement_h__
