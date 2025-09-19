#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "FunctionTemplates.h"
#include "FunctionTemplatesSomeOtherClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string FunctionTemplates::TYPE("FunctionTemplates");

    FunctionTemplates::FunctionTemplates()
    : _reference_counter(1)
    {

    }

    FunctionTemplates::~FunctionTemplates(){}

    void FunctionTemplates::retain()
    {
        ++this->_reference_counter;
    }

    int FunctionTemplates::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool FunctionTemplates::operator ==(const FunctionTemplates& rhs) const
    {
        bool result = true;
        return result;
    }

    bool FunctionTemplates::operator !=(const FunctionTemplates& rhs) const
    {

        return !(*this == rhs);
    }

    FunctionTemplates::FunctionTemplates(const FunctionTemplates& rhs)
    {

        this->operator=(rhs);
    }

    const FunctionTemplates& FunctionTemplates::operator =(const FunctionTemplates& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string FunctionTemplates::get_type() const
    {
        return FunctionTemplates::TYPE;
    }

    void FunctionTemplates::serialize_xml(SerializerXml& serializer) const
    {

    }

    void FunctionTemplates::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void FunctionTemplates::serialize_json(SerializerJson& serializer) const
    {

    }

    void FunctionTemplates::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
