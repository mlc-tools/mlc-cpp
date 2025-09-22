#ifndef __mg_BuilderDamage_h__
#define __mg_BuilderDamage_h__

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
    class ModelEcsBase;

    class BuilderDamage
    {
    public:
        BuilderDamage();
        ~BuilderDamage();
        BuilderDamage& set_damage(float damage);
        int build(ModelEcsBase* model, int attacker_id, int bullet_id, int target_id);
        void retain();
        int release();
        bool operator ==(const BuilderDamage& rhs) const;
        bool operator !=(const BuilderDamage& rhs) const;
        BuilderDamage(const BuilderDamage& rhs);
        const BuilderDamage& operator =(const BuilderDamage& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        float damage;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_BuilderDamage_h__
