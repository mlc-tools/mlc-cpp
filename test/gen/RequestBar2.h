#ifndef __mg_RequestBar2_h__
#define __mg_RequestBar2_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "RequestBar.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
class IVisitorRequest;

    class RequestBar2 : public RequestBar
    {
    public:
        RequestBar2();
        ~RequestBar2();
void accept(IVisitorRequest* visitor);
bool operator ==(const RequestBar2& rhs) const;
bool operator !=(const RequestBar2& rhs) const;
 RequestBar2(const RequestBar2& rhs);
const RequestBar2& operator =(const RequestBar2& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_RequestBar2_h__
