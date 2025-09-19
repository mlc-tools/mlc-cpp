#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "IVisitorRequest.h"
#include "Request.h"
#include "RequestBar.h"
#include "RequestBar2.h"
#include "RequestFoo.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string IVisitorRequest::TYPE("IVisitorRequest");

    IVisitorRequest::IVisitorRequest()
    {

    }

    IVisitorRequest::~IVisitorRequest(){}

    bool IVisitorRequest::operator ==(const IVisitorRequest& rhs) const
    {
        bool result = true;
        return result;
    }

    bool IVisitorRequest::operator !=(const IVisitorRequest& rhs) const
    {

        return !(*this == rhs);
    }

    IVisitorRequest::IVisitorRequest(const IVisitorRequest& rhs)
    {

        this->operator=(rhs);
    }

    const IVisitorRequest& IVisitorRequest::operator =(const IVisitorRequest& rhs)
    {

        return *this;
    }

    std::string IVisitorRequest::get_type() const
    {
        return IVisitorRequest::TYPE;
    }

    void IVisitorRequest::serialize_xml(SerializerXml& serializer) const
    {

    }

    void IVisitorRequest::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void IVisitorRequest::serialize_json(SerializerJson& serializer) const
    {

    }

    void IVisitorRequest::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
