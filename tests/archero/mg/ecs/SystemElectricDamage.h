#ifndef __mg_SystemElectricDamage_h__
#define __mg_SystemElectricDamage_h__

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

    class SystemElectricDamage
    {
    public:
        SystemElectricDamage();
        ~SystemElectricDamage();
        static void add_damage(ModelEcsBase* model, int shooter_id, int target_id, float amount);
        void retain();
        int release();
        bool operator ==(const SystemElectricDamage& rhs) const;
        bool operator !=(const SystemElectricDamage& rhs) const;
        SystemElectricDamage(const SystemElectricDamage& rhs);
        const SystemElectricDamage& operator =(const SystemElectricDamage& rhs);
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

#endif //#ifndef __mg_SystemElectricDamage_h__
