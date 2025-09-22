#ifndef __mg_SystemVampire_h__
#define __mg_SystemVampire_h__

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

    class SystemVampire
    {
    public:
        SystemVampire();
        ~SystemVampire();
        static void on_kill(ModelEcsBase* model, int killer_id, int target_id);
        void retain();
        int release();
        bool operator ==(const SystemVampire& rhs) const;
        bool operator !=(const SystemVampire& rhs) const;
        SystemVampire(const SystemVampire& rhs);
        const SystemVampire& operator =(const SystemVampire& rhs);
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

#endif //#ifndef __mg_SystemVampire_h__
