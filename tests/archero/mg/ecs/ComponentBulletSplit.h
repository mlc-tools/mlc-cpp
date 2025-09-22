#ifndef __mg_ComponentBulletSplit_h__
#define __mg_ComponentBulletSplit_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "DataUnit.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentBulletSplit : public ComponentBase
    {
    public:
        ComponentBulletSplit();
        virtual ~ComponentBulletSplit();
        bool operator ==(const ComponentBulletSplit& rhs) const;
        bool operator !=(const ComponentBulletSplit& rhs) const;
        ComponentBulletSplit(const ComponentBulletSplit& rhs);
        const ComponentBulletSplit& operator =(const ComponentBulletSplit& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        bool was_split;
        int count;
        const DataUnit* to_bullets;
        float split_near_target_radius;
        float timer_to_split;
        bool split_on_remove;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentBulletSplit_h__
