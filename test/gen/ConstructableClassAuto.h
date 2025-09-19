#ifndef __mg_ConstructableClassAuto_h__
#define __mg_ConstructableClassAuto_h__

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

    class ConstructableClassAuto
    {
    public:
        ConstructableClassAuto();
        ~ConstructableClassAuto();
        void retain();
        int release();
        bool operator ==(const ConstructableClassAuto& rhs) const;
        bool operator !=(const ConstructableClassAuto& rhs) const;
        ConstructableClassAuto(const ConstructableClassAuto& rhs);
        const ConstructableClassAuto& operator =(const ConstructableClassAuto& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        int a;
        int b;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ConstructableClassAuto_h__
