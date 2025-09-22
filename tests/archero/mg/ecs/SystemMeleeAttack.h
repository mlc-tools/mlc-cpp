#ifndef __mg_SystemMeleeAttack_h__
#define __mg_SystemMeleeAttack_h__

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
    class ComponentMeleeAttack;
    class ComponentRecharge;
    class ModelEcsBase;
    class Transform;

    class SystemMeleeAttack
    {
    public:
        SystemMeleeAttack();
        ~SystemMeleeAttack();
        void update(ModelEcsBase* model, float dt);
        void try_attack(ModelEcsBase* model, ComponentMeleeAttack* component, ComponentRecharge* component_recharge, Transform* transform);
        void damage(ModelEcsBase* model, ComponentMeleeAttack* component, ComponentRecharge* component_recharge, int target);
        void retain();
        int release();
        bool operator ==(const SystemMeleeAttack& rhs) const;
        bool operator !=(const SystemMeleeAttack& rhs) const;
        SystemMeleeAttack(const SystemMeleeAttack& rhs);
        const SystemMeleeAttack& operator =(const SystemMeleeAttack& rhs);
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

#endif //#ifndef __mg_SystemMeleeAttack_h__
