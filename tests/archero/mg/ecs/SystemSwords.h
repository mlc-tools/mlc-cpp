#ifndef __mg_SystemSwords_h__
#define __mg_SystemSwords_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentSide;
    class ComponentStats;
    class ComponentSword;
    class ComponentSwordCast;
    class ModelEcsBase;
    class Transform;
    class Vector;

    class SystemSwords
    {
    public:
        SystemSwords();
        ~SystemSwords();
        void update(ModelEcsBase* model, float dt);
        void position_swords(ModelEcsBase* model, ComponentSwordCast* sword_cast, Transform* player_transform);
        std::vector<Vector> generate_base_points(ModelEcsBase* model, int count);
        void appearance(ModelEcsBase* model, ComponentSword* sword, Transform* transform, ComponentSwordCast* sword_cast, Transform* player_transform);
        int find_target(ModelEcsBase* model, ComponentSword* sword, Transform* transform, ComponentStats* stats, ComponentSide* side);
        void prepare_to_fly(ModelEcsBase* model, ComponentSword* sword, Transform* transform);
        void fly(ModelEcsBase* model, ComponentSword* sword, Transform* transform);
        bool check_damage(ModelEcsBase* model, ComponentSword* sword, Transform* transform, Transform* target_transform);
        void damage(ModelEcsBase* model, ComponentSword* sword, int target_id);
        void retain();
        int release();
        bool operator ==(const SystemSwords& rhs) const;
        bool operator !=(const SystemSwords& rhs) const;
        SystemSwords(const SystemSwords& rhs);
        const SystemSwords& operator =(const SystemSwords& rhs);
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

#endif //#ifndef __mg_SystemSwords_h__
