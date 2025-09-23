#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "IVisitorResponse.h"
#include "Response.h"
#include "ResponseError.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ResponseError::TYPE("ResponseError");

    ResponseError::ResponseError(const std::string& error)
    : error("")
    {
        this->error = error;
    }

    ResponseError::~ResponseError()
    {
    }

    void ResponseError::accept(IVisitorResponse* visitor)
    {
        visitor->visit(this);
    }

    bool ResponseError::operator ==(const ResponseError& rhs) const
    {
        bool result = this->Response::operator ==(rhs);
        result = result && this->error == rhs.error;
        return result;
    }

    bool ResponseError::operator !=(const ResponseError& rhs) const
    {
        return !(*this == rhs);
    }

    ResponseError::ResponseError(const ResponseError& rhs)
    {
        this->operator=(rhs);
    }

    const ResponseError& ResponseError::operator =(const ResponseError& rhs)
    {
        this->Response::operator=(rhs);
        this->error = rhs.error;
        return *this;
    }

    std::string ResponseError::get_type() const
    {
        return ResponseError::TYPE;
    }

    void ResponseError::serialize_xml(SerializerXml& serializer) const
    {
        Response::serialize_xml(serializer);
        serializer.serialize(error, "error", std::string(""));
    }

    void ResponseError::deserialize_xml(DeserializerXml& deserializer)
    {
        Response::deserialize_xml(deserializer);
        deserializer.deserialize(error, "error", std::string(""));
    }

    void ResponseError::serialize_json(SerializerJson& serializer) const
    {
        Response::serialize_json(serializer);
        serializer.serialize(error, "error", std::string(""));
    }

    void ResponseError::deserialize_json(DeserializerJson& deserializer)
    {
        Response::deserialize_json(deserializer);
        deserializer.deserialize(error, "error", std::string(""));
    }

} //namespace mg
