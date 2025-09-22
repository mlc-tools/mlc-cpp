#ifndef __mg_SystemDeath_h__
#define __mg_SystemDeath_h__

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
    class ComponentHealth;
    class ModelEcsBase;

    class SystemDeath
    {
    public:
        SystemDeath();
        ~SystemDeath();
        void update(ModelEcsBase* model, float dt);
        void clean(ModelEcsBase* model);
        void death(ModelEcsBase* model, ComponentHealth* health);
        bool is_ressurection(ModelEcsBase* model, ComponentHealth* health);
        void retain();
        int release();
        bool operator ==(const SystemDeath& rhs) const;
        bool operator !=(const SystemDeath& rhs) const;
        SystemDeath(const SystemDeath& rhs);
        const SystemDeath& operator =(const SystemDeath& rhs);
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

#endif //#ifndef __mg_SystemDeath_h__
