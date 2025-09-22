#ifndef __mg_SystemSwordsCast_h__
#define __mg_SystemSwordsCast_h__

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
    class ComponentSide;
    class ComponentStats;
    class ComponentSword;
    class ComponentSwordCast;
    class ModelEcsBase;
    class Transform;

    class SystemSwordsCast
    {
    public:
        SystemSwordsCast();
        ~SystemSwordsCast();
        void update(ModelEcsBase* model, float dt);
        void clean(ModelEcsBase* model);
        void remove_sword(ModelEcsBase* model, ComponentSword* sword);
        void cast_sword(ModelEcsBase* model, ComponentSwordCast* sword_cast, Transform* transform, ComponentStats* stats, ComponentSide* side, int count);
        void retain();
        int release();
        bool operator ==(const SystemSwordsCast& rhs) const;
        bool operator !=(const SystemSwordsCast& rhs) const;
        SystemSwordsCast(const SystemSwordsCast& rhs);
        const SystemSwordsCast& operator =(const SystemSwordsCast& rhs);
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

#endif //#ifndef __mg_SystemSwordsCast_h__
