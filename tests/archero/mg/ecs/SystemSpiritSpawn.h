#ifndef __mg_SystemSpiritSpawn_h__
#define __mg_SystemSpiritSpawn_h__

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
    class ComponentSpawnSpirit;
    class ModelEcsBase;

    class SystemSpiritSpawn
    {
    public:
        SystemSpiritSpawn();
        ~SystemSpiritSpawn();
        void update(ModelEcsBase* model, float dt);
        void create_spirit(ModelEcsBase* model, ComponentSpawnSpirit* spawn);
        void retain();
        int release();
        bool operator ==(const SystemSpiritSpawn& rhs) const;
        bool operator !=(const SystemSpiritSpawn& rhs) const;
        SystemSpiritSpawn(const SystemSpiritSpawn& rhs);
        const SystemSpiritSpawn& operator =(const SystemSpiritSpawn& rhs);
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

#endif //#ifndef __mg_SystemSpiritSpawn_h__
