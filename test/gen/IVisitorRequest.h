#ifndef __mg_IVisitorRequest_h__
#define __mg_IVisitorRequest_h__

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
    class Request;
    class RequestBar2;
    class RequestBar;
    class RequestFoo;

    class IVisitorRequest
    {
    public:
        IVisitorRequest();
        virtual ~IVisitorRequest();
        virtual void visit(Request* ctx) = 0;
        virtual void visit(RequestBar* ctx) = 0;
        virtual void visit(RequestBar2* ctx) = 0;
        virtual void visit(RequestFoo* ctx) = 0;
        virtual bool operator ==(const IVisitorRequest& rhs) const;
        virtual bool operator !=(const IVisitorRequest& rhs) const;
        IVisitorRequest(const IVisitorRequest& rhs);
        virtual const IVisitorRequest& operator =(const IVisitorRequest& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_IVisitorRequest_h__
