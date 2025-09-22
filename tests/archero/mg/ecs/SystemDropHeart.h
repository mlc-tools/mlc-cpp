#ifndef __mg_SystemDropHeart_h__
#define __mg_SystemDropHeart_h__

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
    class ComponentHeartAddStats;
    class ModelEcsBase;

    class SystemDropHeart
    {
    public:
        SystemDropHeart();
        ~SystemDropHeart();
        void update(ModelEcsBase* model, float dt);
        void check_lucky_heart(ModelEcsBase* model, ComponentHeartAddStats* component);
        void clean(ModelEcsBase* model);
        void retain();
        int release();
        bool operator ==(const SystemDropHeart& rhs) const;
        bool operator !=(const SystemDropHeart& rhs) const;
        SystemDropHeart(const SystemDropHeart& rhs);
        const SystemDropHeart& operator =(const SystemDropHeart& rhs);
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

#endif //#ifndef __mg_SystemDropHeart_h__
