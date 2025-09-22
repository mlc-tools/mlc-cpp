#ifndef __mg_SystemPush_h__
#define __mg_SystemPush_h__

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

    class SystemPush
    {
    public:
        SystemPush();
        ~SystemPush();
        void update(ModelEcsBase* model, float dt);
        void clean(ModelEcsBase* model);
        void retain();
        int release();
        bool operator ==(const SystemPush& rhs) const;
        bool operator !=(const SystemPush& rhs) const;
        SystemPush(const SystemPush& rhs);
        const SystemPush& operator =(const SystemPush& rhs);
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

#endif //#ifndef __mg_SystemPush_h__
