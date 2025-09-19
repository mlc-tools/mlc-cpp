#ifndef __mg_RequestBar_h__
#define __mg_RequestBar_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Request.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class IVisitorRequest;

    class RequestBar : public Request
    {
    public:
        RequestBar();
        ~RequestBar();
        virtual void accept(IVisitorRequest* visitor) override;
        bool operator ==(const RequestBar& rhs) const;
        bool operator !=(const RequestBar& rhs) const;
        RequestBar(const RequestBar& rhs);
        const RequestBar& operator =(const RequestBar& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_RequestBar_h__
