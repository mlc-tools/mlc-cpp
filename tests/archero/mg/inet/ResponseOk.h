#ifndef __mg_ResponseOk_h__
#define __mg_ResponseOk_h__

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

    class ResponseOk : public Response
    {
    public:
        ResponseOk();
        virtual ~ResponseOk();
        virtual void accept(IVisitorResponse* visitor) override;
        bool operator ==(const ResponseOk& rhs) const;
        bool operator !=(const ResponseOk& rhs) const;
        ResponseOk(const ResponseOk& rhs);
        const ResponseOk& operator =(const ResponseOk& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ResponseOk_h__
