#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "IVisitorResponse.h"
#include "Response.h"
#include "ResponseError.h"
#include "ResponseOk.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string IVisitorResponse::TYPE("IVisitorResponse");

    IVisitorResponse::IVisitorResponse()
    {

    }

    IVisitorResponse::~IVisitorResponse()
    {
    }

    bool IVisitorResponse::operator ==(const IVisitorResponse& rhs) const
    {
        bool result = true;
        return result;
    }

    bool IVisitorResponse::operator !=(const IVisitorResponse& rhs) const
    {
        return !(*this == rhs);
    }

    IVisitorResponse::IVisitorResponse(const IVisitorResponse& rhs)
    {
        this->operator=(rhs);
    }

    const IVisitorResponse& IVisitorResponse::operator =(const IVisitorResponse& rhs)
    {
        return *this;
    }

    std::string IVisitorResponse::get_type() const
    {
        return IVisitorResponse::TYPE;
    }

    void IVisitorResponse::serialize_xml(SerializerXml& serializer) const
    {

    }

    void IVisitorResponse::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void IVisitorResponse::serialize_json(SerializerJson& serializer) const
    {

    }

    void IVisitorResponse::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
