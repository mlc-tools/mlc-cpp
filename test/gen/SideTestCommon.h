#ifndef __mg_SideTestCommon_h__
#define __mg_SideTestCommon_h__

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

    class SideTestCommon
    {
    public:
        SideTestCommon();
        ~SideTestCommon();
        std::string get_value();
        void retain();
        int release();
        bool operator ==(const SideTestCommon& rhs) const;
        bool operator !=(const SideTestCommon& rhs) const;
        SideTestCommon(const SideTestCommon& rhs);
        const SideTestCommon& operator =(const SideTestCommon& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string client_value;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_SideTestCommon_h__
