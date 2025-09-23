#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CommandBase.h"
#include "IVisitorResponse.h"
#include "Response.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Response::TYPE("Response");

    Response::Response()
    {

    }

    Response::~Response()
    {
    }

    void Response::accept(IVisitorResponse* visitor)
    {
        visitor->visit(this);
    }

    bool Response::operator ==(const Response& rhs) const
    {
        bool result = this->CommandBase::operator ==(rhs);
        return result;
    }

    bool Response::operator !=(const Response& rhs) const
    {
        return !(*this == rhs);
    }

    Response::Response(const Response& rhs)
    {
        this->operator=(rhs);
    }

    const Response& Response::operator =(const Response& rhs)
    {
        this->CommandBase::operator=(rhs);
        return *this;
    }

    std::string Response::get_type() const
    {
        return Response::TYPE;
    }

    void Response::serialize_xml(SerializerXml& serializer) const
    {
        CommandBase::serialize_xml(serializer);
    }

    void Response::deserialize_xml(DeserializerXml& deserializer)
    {
        CommandBase::deserialize_xml(deserializer);
    }

    void Response::serialize_json(SerializerJson& serializer) const
    {
        CommandBase::serialize_json(serializer);
    }

    void Response::deserialize_json(DeserializerJson& deserializer)
    {
        CommandBase::deserialize_json(deserializer);
    }

} //namespace mg
