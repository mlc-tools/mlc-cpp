#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "IVisitorRequest.h"
#include "RequestBar.h"
#include "RequestBar2.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string RequestBar2::TYPE("RequestBar2");

    RequestBar2::RequestBar2()
    {

    }

    RequestBar2::~RequestBar2(){}

    void RequestBar2::accept(IVisitorRequest* visitor)
    {
        visitor->visit(this);
    }

    bool RequestBar2::operator ==(const RequestBar2& rhs) const
    {

        bool result = this->RequestBar::operator ==(rhs);
        return result;
    }

    bool RequestBar2::operator !=(const RequestBar2& rhs) const
    {

        return !(*this == rhs);
    }

    RequestBar2::RequestBar2(const RequestBar2& rhs)
    {

        this->operator=(rhs);
    }

    const RequestBar2& RequestBar2::operator =(const RequestBar2& rhs)
    {

        this->RequestBar::operator=(rhs);
        return *this;
    }

    std::string RequestBar2::get_type() const
    {
        return RequestBar2::TYPE;
    }

    void RequestBar2::serialize_xml(SerializerXml& serializer) const
    {

    }

    void RequestBar2::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void RequestBar2::serialize_json(SerializerJson& serializer) const
    {

    }

    void RequestBar2::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
