#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "IVisitorResponse.h"
#include "ResponseOk.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ResponseOk::TYPE("ResponseOk");

    ResponseOk::ResponseOk()
    {

    }

    ResponseOk::~ResponseOk()
    {
    }

    void ResponseOk::accept(IVisitorResponse* visitor)
    {
        visitor->visit(this);
    }

    bool ResponseOk::operator ==(const ResponseOk& rhs) const
    {
        bool result = this->Response::operator ==(rhs);
        return result;
    }

    bool ResponseOk::operator !=(const ResponseOk& rhs) const
    {
        return !(*this == rhs);
    }

    ResponseOk::ResponseOk(const ResponseOk& rhs)
    {
        this->operator=(rhs);
    }

    const ResponseOk& ResponseOk::operator =(const ResponseOk& rhs)
    {
        this->Response::operator=(rhs);
        return *this;
    }

    std::string ResponseOk::get_type() const
    {
        return ResponseOk::TYPE;
    }

    void ResponseOk::serialize_xml(SerializerXml& serializer) const
    {
        Response::serialize_xml(serializer);
    }

    void ResponseOk::deserialize_xml(DeserializerXml& deserializer)
    {
        Response::deserialize_xml(deserializer);
    }

    void ResponseOk::serialize_json(SerializerJson& serializer) const
    {
        Response::serialize_json(serializer);
    }

    void ResponseOk::deserialize_json(DeserializerJson& deserializer)
    {
        Response::deserialize_json(deserializer);
    }

} //namespace mg
