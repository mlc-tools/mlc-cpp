#ifndef __mg_SystemBullet_h__
#define __mg_SystemBullet_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentBullet;
    class ComponentBulletSplit;
    class ComponentSide;
    class ComponentStats;
    class ModelEcsBase;
    class Transform;

    class SystemBullet
    {
    public:
        SystemBullet();
        ~SystemBullet();
        void update(ModelEcsBase* model, float dt);
        void update_bullet(ModelEcsBase* model, ComponentBullet* component, Transform* transform, ComponentStats* bullet_stats, ComponentSide* side_bullet, float dt);
        void update_split(ModelEcsBase* model, float dt);
        void update_lasers(ModelEcsBase* model, float dt);
        bool try_ricochet(ModelEcsBase* model, ComponentBullet* component, ComponentSide* side_bullet, Transform* transform, int prev_target_id);
        static void split_on_hit(ModelEcsBase* model, ComponentBullet* bullet, Transform* transform, int target_id);
        void damage(ModelEcsBase* model, ComponentBullet* component, int target_id, float mul=1.f);
        void damage_massive(ModelEcsBase* model, ComponentBullet* component);
        static void remove_bullet(ModelEcsBase* model, ComponentBullet* bullet);
        static void split_bullet(ModelEcsBase* model, ComponentBullet* bullet, ComponentBulletSplit* component_split);
        void retain();
        int release();
        bool operator ==(const SystemBullet& rhs) const;
        bool operator !=(const SystemBullet& rhs) const;
        SystemBullet(const SystemBullet& rhs);
        const SystemBullet& operator =(const SystemBullet& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_SystemBullet_h__
