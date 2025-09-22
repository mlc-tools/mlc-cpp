#ifndef __mg_ComponentBullet_h__
#define __mg_ComponentBullet_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BulletType.h"
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

    class ComponentBullet : public ComponentBase
    {
    public:
        ComponentBullet();
        virtual ~ComponentBullet();
        bool operator ==(const ComponentBullet& rhs) const;
        bool operator !=(const ComponentBullet& rhs) const;
        ComponentBullet(const ComponentBullet& rhs);
        const ComponentBullet& operator =(const ComponentBullet& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int shooter_id;
        int target_id;
        float damage;
        const DataUnit* data;
        bool clean;
        bool through_walls;
        bool pierce;
        BulletType bullet_type;
        int ricochet_bounce;
        int ricochet_wall;
        bool split_on_hit;
        int ignore_target_id;
        float ignore_target_timer;
        bool always_life;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentBullet_h__
