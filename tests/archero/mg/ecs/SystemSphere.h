#ifndef __mg_SystemSphere_h__
#define __mg_SystemSphere_h__

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
    class ComponentSphereSpawn;
    class ModelEcsBase;

    class SystemSphere
    {
    public:
        SystemSphere();
        ~SystemSphere();
        void update(ModelEcsBase* model, float dt);
        void check_spawn(ModelEcsBase* model);
        void spawn(ModelEcsBase* model, ComponentSphereSpawn* spawn);
        void update_positions(ModelEcsBase* model, float dt);
        void retain();
        int release();
        bool operator ==(const SystemSphere& rhs) const;
        bool operator !=(const SystemSphere& rhs) const;
        SystemSphere(const SystemSphere& rhs);
        const SystemSphere& operator =(const SystemSphere& rhs);
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

#endif //#ifndef __mg_SystemSphere_h__
