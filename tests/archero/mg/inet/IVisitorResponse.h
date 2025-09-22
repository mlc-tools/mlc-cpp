#ifndef __mg_IVisitorResponse_h__
#define __mg_IVisitorResponse_h__

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
    class Response;
    class ResponseError;
    class ResponseOk;

    class IVisitorResponse
    {
    public:
        IVisitorResponse();
        virtual ~IVisitorResponse();
        virtual void visit(Response* ctx) = 0;
        virtual void visit(ResponseError* ctx) = 0;
        virtual void visit(ResponseOk* ctx) = 0;
        virtual bool operator ==(const IVisitorResponse& rhs) const;
        virtual bool operator !=(const IVisitorResponse& rhs) const;
        IVisitorResponse(const IVisitorResponse& rhs);
        virtual const IVisitorResponse& operator =(const IVisitorResponse& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_IVisitorResponse_h__
