#ifndef __mg_SystemMeteor_h__
#define __mg_SystemMeteor_h__

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

    class SystemMeteor
    {
    public:
        SystemMeteor();
        ~SystemMeteor();
        void update(ModelEcsBase* model, float dt);
        void dispatch_periodic_meteor(ModelEcsBase* model, float dt);
        static void create_meteor(ModelEcsBase* model, int shooter_id, int count, int target_id);
        static int get_random_enemy(ModelEcsBase* model, int shooter_id);
        void retain();
        int release();
        bool operator ==(const SystemMeteor& rhs) const;
        bool operator !=(const SystemMeteor& rhs) const;
        SystemMeteor(const SystemMeteor& rhs);
        const SystemMeteor& operator =(const SystemMeteor& rhs);
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

#endif //#ifndef __mg_SystemMeteor_h__
