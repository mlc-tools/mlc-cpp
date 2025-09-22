#ifndef __mg_SystemExp_h__
#define __mg_SystemExp_h__

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

    class SystemExp
    {
    public:
        SystemExp();
        ~SystemExp();
        void update(ModelEcsBase* model, float dt);
        void clean(ModelEcsBase* model);
        void create_exp(ModelEcsBase* model);
        void retain();
        int release();
        bool operator ==(const SystemExp& rhs) const;
        bool operator !=(const SystemExp& rhs) const;
        SystemExp(const SystemExp& rhs);
        const SystemExp& operator =(const SystemExp& rhs);
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

#endif //#ifndef __mg_SystemExp_h__
