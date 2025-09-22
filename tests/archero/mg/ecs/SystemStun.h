#ifndef __mg_SystemStun_h__
#define __mg_SystemStun_h__

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

    class SystemStun
    {
    public:
        SystemStun();
        ~SystemStun();
        void update(ModelEcsBase* model, float dt);
        static void apply(ModelEcsBase* model, int target_id, float duration);
        void retain();
        int release();
        bool operator ==(const SystemStun& rhs) const;
        bool operator !=(const SystemStun& rhs) const;
        SystemStun(const SystemStun& rhs);
        const SystemStun& operator =(const SystemStun& rhs);
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

#endif //#ifndef __mg_SystemStun_h__
