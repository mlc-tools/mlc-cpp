#ifndef __mg_Response_h__
#define __mg_Response_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "CommandBase.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class IVisitorResponse;

    class Response : public CommandBase
    {
    public:
        Response();
        virtual ~Response();
        virtual void accept(IVisitorResponse* visitor) override;
        bool operator ==(const Response& rhs) const;
        bool operator !=(const Response& rhs) const;
        Response(const Response& rhs);
        const Response& operator =(const Response& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Response_h__
