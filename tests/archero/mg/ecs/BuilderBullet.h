#ifndef __mg_BuilderBullet_h__
#define __mg_BuilderBullet_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "DataUnit.h"
#include "Side.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentStats;
    class ModelEcsBase;
    class Side;
    class Vector;

    class BuilderBullet
    {
    public:
        BuilderBullet(int shooter_id=0);
        ~BuilderBullet();
        BuilderBullet& set_name(const std::string& value);
        BuilderBullet& set_target_id(int value);
        BuilderBullet& set_damage(float value);
        BuilderBullet& set_side(Side value);
        BuilderBullet& set_position_create(const Vector& value);
        BuilderBullet& set_position_to_shoot(const Vector& value);
        BuilderBullet& set_direction(const Vector& value);
        BuilderBullet& set_offset_create(const Vector& value);
        BuilderBullet& set_deviation_from_direction(float value);
        BuilderBullet& set_spine_bone_of_create_bullet(const std::string& value);
        int build(ModelEcsBase* model);
        Vector get_position_to_create(ModelEcsBase* model) const;
        Vector get_target_position(ModelEcsBase* model) const;
        Vector get_preemption(const Vector& target_pos, const Vector& target_direction, float target_speed, const Vector& shooter_pos, float bullet_speed) const;
        Vector get_direction(ModelEcsBase* model) const;
        std::string get_bullet_name(ModelEcsBase* model, ComponentStats* stats) const;
        void retain();
        int release();
        bool operator ==(const BuilderBullet& rhs) const;
        bool operator !=(const BuilderBullet& rhs) const;
        BuilderBullet(const BuilderBullet& rhs);
        const BuilderBullet& operator =(const BuilderBullet& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        int shooter_id;
        int target_id;
        std::string bullet_name;
        Vector position_create;
        Vector position_to_shoot;
        Vector direction;
        Vector offset_create;
        float deviation_from_direction;
        Side side;
        std::string spine_bone_of_create_bullet;
        const DataUnit* data;
        float damage;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_BuilderBullet_h__
