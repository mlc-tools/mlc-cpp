#ifndef __mg_SystemCreateUnitsOnDeath_h__
#define __mg_SystemCreateUnitsOnDeath_h__

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
    class ComponentCreateUnitsOnDeath;
    class ModelEcsBase;

    class SystemCreateUnitsOnDeath
    {
    public:
        SystemCreateUnitsOnDeath();
        ~SystemCreateUnitsOnDeath();
        void update(ModelEcsBase* model, float dt);
        void create_units(ModelEcsBase* model, ComponentCreateUnitsOnDeath* component);
        void retain();
        int release();
        bool operator ==(const SystemCreateUnitsOnDeath& rhs) const;
        bool operator !=(const SystemCreateUnitsOnDeath& rhs) const;
        SystemCreateUnitsOnDeath(const SystemCreateUnitsOnDeath& rhs);
        const SystemCreateUnitsOnDeath& operator =(const SystemCreateUnitsOnDeath& rhs);
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

#endif //#ifndef __mg_SystemCreateUnitsOnDeath_h__
