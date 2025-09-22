#ifndef __mg_ComponentSphere_h__
#define __mg_ComponentSphere_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
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

    class ComponentSphere : public ComponentBase
    {
    public:
        ComponentSphere();
        virtual ~ComponentSphere();
        bool operator ==(const ComponentSphere& rhs) const;
        bool operator !=(const ComponentSphere& rhs) const;
        ComponentSphere(const ComponentSphere& rhs);
        const ComponentSphere& operator =(const ComponentSphere& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int hero_id;
        float angle;
        float radius;
        float speed;
        std::map<int, float> ignore_targets;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSphere_h__
