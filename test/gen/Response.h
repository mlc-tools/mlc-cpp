#ifndef __mg_Response_h__
#define __mg_Response_h__

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

    class Response
    {
    public:
        Response();
        ~Response();
        void retain();
        int release();
        bool operator ==(const Response& rhs) const;
        bool operator !=(const Response& rhs) const;
        Response(const Response& rhs);
        const Response& operator =(const Response& rhs);
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

#endif // __mg_Response_h__
