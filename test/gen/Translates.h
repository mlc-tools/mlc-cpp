#ifndef __mg_Translates_h__
#define __mg_Translates_h__

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

    class Translates
    {
    public:
        Translates();
        ~Translates();
        void dummy_function();
        void new_function();
        void retain();
        int release();
        bool operator ==(const Translates& rhs) const;
        bool operator !=(const Translates& rhs) const;
        Translates(const Translates& rhs);
        const Translates& operator =(const Translates& rhs);
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
} // namespace mg

#endif // __mg_Translates_h__
