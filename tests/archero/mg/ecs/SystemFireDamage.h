#ifndef __mg_SystemFireDamage_h__
#define __mg_SystemFireDamage_h__

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

    class SystemFireDamage
    {
    public:
        SystemFireDamage();
        ~SystemFireDamage();
        static void add_damage(ModelEcsBase* model, int shooter_id, int target_id, float amount);
        void update(ModelEcsBase* model, float dt);
        void retain();
        int release();
        bool operator ==(const SystemFireDamage& rhs) const;
        bool operator !=(const SystemFireDamage& rhs) const;
        SystemFireDamage(const SystemFireDamage& rhs);
        const SystemFireDamage& operator =(const SystemFireDamage& rhs);
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

#endif //#ifndef __mg_SystemFireDamage_h__
