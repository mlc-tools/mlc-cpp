#ifndef __mg_SystemSpawn_h__
#define __mg_SystemSpawn_h__

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
    class ComponentSpawn;
    class ModelEcsBase;

    class SystemSpawn
    {
    public:
        SystemSpawn();
        ~SystemSpawn();
        void update(ModelEcsBase* model, float dt);
        void spawn_unit(ModelEcsBase* model, ComponentSpawn* spawn);
        void retain();
        int release();
        bool operator ==(const SystemSpawn& rhs) const;
        bool operator !=(const SystemSpawn& rhs) const;
        SystemSpawn(const SystemSpawn& rhs);
        const SystemSpawn& operator =(const SystemSpawn& rhs);
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

#endif //#ifndef __mg_SystemSpawn_h__
