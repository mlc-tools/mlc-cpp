#ifndef __mg_RequestAcceptor_h__
#define __mg_RequestAcceptor_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "IVisitorRequest.h"
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

    class RequestAcceptor : public IVisitorRequest
    {
    public:
        RequestAcceptor();
        ~RequestAcceptor();
        void visit(Request* ctx);
        void visit(RequestAcceptor* ctx);
        void visit(RequestFoo* ctx);
        void visit(RequestBar* ctx);
        void visit(RequestBar2* ctx);
        bool operator ==(const RequestAcceptor& rhs) const;
        bool operator !=(const RequestAcceptor& rhs) const;
        RequestAcceptor(const RequestAcceptor& rhs);
        const RequestAcceptor& operator =(const RequestAcceptor& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int base;
        int foo;
        int bar;
        int bar2;
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_RequestAcceptor_h__
