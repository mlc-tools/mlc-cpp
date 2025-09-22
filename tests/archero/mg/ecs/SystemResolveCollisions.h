#ifndef __mg_SystemResolveCollisions_h__
#define __mg_SystemResolveCollisions_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../Observable.h"
#include "Vector.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;
    class Transform;
    class Vector;

    class SystemResolveCollisions
    {
    public:
        SystemResolveCollisions();
        ~SystemResolveCollisions();
        void update(ModelEcsBase* model, float dt);
        void resole_collision_with_poly(ModelEcsBase* model);
        void notify(int id, int target_id);
        bool is_collision(Transform* a, Transform* b, float size_a, float size_b);
        bool is_collision(Transform* a, Transform* b, float size_a, float size_b, const Vector& target_offset);
        void resole_collision(ModelEcsBase* model, Transform* transform_a, Transform* transform_b, float size_a, float size_b);
        static bool can_move(ModelEcsBase* model, int entity_id, const Vector& pos, float radius);
        static bool is_wall(ModelEcsBase* model, int entity_id, const Vector& pos, float radius);
        static Vector find_collision_ray_and_walls(ModelEcsBase* model, const Vector& ray_pos, const Vector& ray_direction, float min_distance);
        void retain();
        int release();
        bool operator ==(const SystemResolveCollisions& rhs) const;
        bool operator !=(const SystemResolveCollisions& rhs) const;
        SystemResolveCollisions(const SystemResolveCollisions& rhs);
        const SystemResolveCollisions& operator =(const SystemResolveCollisions& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        static std::map<int, Observable<void(int)>> event_collision;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_SystemResolveCollisions_h__
