#ifndef __mg_RequestFoo_h__
#define __mg_RequestFoo_h__

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

    class RequestFoo : public Request
    {
    public:
        RequestFoo();
        ~RequestFoo();
void accept(IVisitorRequest* visitor);
bool operator ==(const RequestFoo& rhs) const;
bool operator !=(const RequestFoo& rhs) const;
 RequestFoo(const RequestFoo& rhs);
const RequestFoo& operator =(const RequestFoo& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_RequestFoo_h__
