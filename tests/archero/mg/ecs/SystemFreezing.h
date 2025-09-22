#ifndef __mg_SystemFreezing_h__
#define __mg_SystemFreezing_h__

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
    class ComponentStats;
    class ModelEcsBase;

    class SystemFreezing
    {
    public:
        SystemFreezing();
        ~SystemFreezing();
        static void add_damage(ModelEcsBase* model, int target_id, ComponentStats* target_stats, float damage);
        void update(ModelEcsBase* model, float dt);
        void retain();
        int release();
        bool operator ==(const SystemFreezing& rhs) const;
        bool operator !=(const SystemFreezing& rhs) const;
        SystemFreezing(const SystemFreezing& rhs);
        const SystemFreezing& operator =(const SystemFreezing& rhs);
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

#endif //#ifndef __mg_SystemFreezing_h__
