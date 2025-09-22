#ifndef __mg_SystemActions_h__
#define __mg_SystemActions_h__

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
    class ComponentActions;
    class ModelEcsBase;

    class SystemActions
    {
    public:
        SystemActions();
        ~SystemActions();
        void update(ModelEcsBase* model, float dt);
        void switch_to_next(ModelEcsBase* model, ComponentActions* component);
        void retain();
        int release();
        bool operator ==(const SystemActions& rhs) const;
        bool operator !=(const SystemActions& rhs) const;
        SystemActions(const SystemActions& rhs);
        const SystemActions& operator =(const SystemActions& rhs);
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

#endif //#ifndef __mg_SystemActions_h__
