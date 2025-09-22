#ifndef __mg_SystemAuraDamage_h__
#define __mg_SystemAuraDamage_h__

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

    class SystemAuraDamage
    {
    public:
        SystemAuraDamage();
        ~SystemAuraDamage();
        void update(ModelEcsBase* model, float dt);
        void retain();
        int release();
        bool operator ==(const SystemAuraDamage& rhs) const;
        bool operator !=(const SystemAuraDamage& rhs) const;
        SystemAuraDamage(const SystemAuraDamage& rhs);
        const SystemAuraDamage& operator =(const SystemAuraDamage& rhs);
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

#endif //#ifndef __mg_SystemAuraDamage_h__
