#ifndef __mg_SystemRemoveDamage_h__
#define __mg_SystemRemoveDamage_h__

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

    class SystemRemoveDamage
    {
    public:
        SystemRemoveDamage();
        ~SystemRemoveDamage();
        void update(ModelEcsBase* model, float dt);
        void clean(ModelEcsBase* model);
        void retain();
        int release();
        bool operator ==(const SystemRemoveDamage& rhs) const;
        bool operator !=(const SystemRemoveDamage& rhs) const;
        SystemRemoveDamage(const SystemRemoveDamage& rhs);
        const SystemRemoveDamage& operator =(const SystemRemoveDamage& rhs);
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

#endif //#ifndef __mg_SystemRemoveDamage_h__
