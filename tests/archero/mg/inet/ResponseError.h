#ifndef __mg_ResponseError_h__
#define __mg_ResponseError_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Response.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class IVisitorResponse;

    class ResponseError : public Response
    {
    public:
        ResponseError(const std::string& error="");
        virtual ~ResponseError();
        virtual void accept(IVisitorResponse* visitor) override;
        bool operator ==(const ResponseError& rhs) const;
        bool operator !=(const ResponseError& rhs) const;
        ResponseError(const ResponseError& rhs);
        const ResponseError& operator =(const ResponseError& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::string error;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ResponseError_h__
