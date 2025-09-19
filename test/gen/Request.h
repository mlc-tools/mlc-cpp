#ifndef __mg_Request_h__
#define __mg_Request_h__

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
    class IVisitorRequest;

    class Request
    {
    public:
        Request();
        ~Request();
        virtual void accept(IVisitorRequest* visitor);
        void retain();
        int release();
        bool operator ==(const Request& rhs) const;
        bool operator !=(const Request& rhs) const;
        Request(const Request& rhs);
        const Request& operator =(const Request& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_Request_h__
