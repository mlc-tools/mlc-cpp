#ifndef __mg_SystemDamage_h__
#define __mg_SystemDamage_h__

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
    class ComponentDamage;
    class ComponentStats;
    class ModelEcsBase;

    class SystemDamage
    {
    public:
        SystemDamage();
        ~SystemDamage();
        void update(ModelEcsBase* model, float dt);
        static float compute_damage(ModelEcsBase* model, int shooter_id, int target_id, ComponentStats* stats_attacker=nullptr);
        static float is_dodge(ModelEcsBase* model, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static float compute_freeze_chance(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static float compute_stun_chance(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static float compute_stun_duration(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static float compute_freeze_damage(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static float compute_fire_damage(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static float compute_electric_damage(ModelEcsBase* model, ComponentDamage* component_damage, int shooter_id, int target_id, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static float compute_future_damage(ModelEcsBase* model, int shooter_id, int bullet_id, int target_id);
        void check_effects_after_damage(ModelEcsBase* model, ComponentDamage* damage, ComponentStats* stats_attacker, ComponentStats* stats_target);
        static void check_effects_on_kill(ModelEcsBase* model, ComponentStats* stats_attacker, ComponentStats* stats_target);
        void retain();
        int release();
        bool operator ==(const SystemDamage& rhs) const;
        bool operator !=(const SystemDamage& rhs) const;
        SystemDamage(const SystemDamage& rhs);
        const SystemDamage& operator =(const SystemDamage& rhs);
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

#endif //#ifndef __mg_SystemDamage_h__
