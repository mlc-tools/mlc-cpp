#ifndef __mg_SystemShoot_h__
#define __mg_SystemShoot_h__

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
    class ComponentRecharge;
    class ComponentSide;
    class ModelEcsBase;
    class Transform;

    class SystemShoot
    {
    public:
        SystemShoot();
        ~SystemShoot();
        void update(ModelEcsBase* model, float dt);
        void try_shoot(ModelEcsBase* model, ComponentRecharge* component);
        int find_target(ModelEcsBase* model, ComponentRecharge* component);
        static int get_nearest_entity_in_radius(ModelEcsBase* model, int shooter, float radius, ComponentSide* side_shooter, Transform* transform_shooter, bool with_future=true, int prev_target=-1);
        void shoot(ModelEcsBase* model, ComponentRecharge* component, int target);
        void retain();
        int release();
        bool operator ==(const SystemShoot& rhs) const;
        bool operator !=(const SystemShoot& rhs) const;
        SystemShoot(const SystemShoot& rhs);
        const SystemShoot& operator =(const SystemShoot& rhs);
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

#endif //#ifndef __mg_SystemShoot_h__
