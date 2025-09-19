#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "FunctionTemplatesSomeOtherClass.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string FunctionTemplatesSomeOtherClass::TYPE("FunctionTemplatesSomeOtherClass");

    FunctionTemplatesSomeOtherClass::FunctionTemplatesSomeOtherClass()
    : _reference_counter(1)
    {

    }

    FunctionTemplatesSomeOtherClass::~FunctionTemplatesSomeOtherClass(){}

    void FunctionTemplatesSomeOtherClass::some_method()
    {

    }

    void FunctionTemplatesSomeOtherClass::retain()
    {
        ++this->_reference_counter;
    }

    int FunctionTemplatesSomeOtherClass::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool FunctionTemplatesSomeOtherClass::operator ==(const FunctionTemplatesSomeOtherClass& rhs) const
    {
        bool result = true;
        return result;
    }

    bool FunctionTemplatesSomeOtherClass::operator !=(const FunctionTemplatesSomeOtherClass& rhs) const
    {

        return !(*this == rhs);
    }

    FunctionTemplatesSomeOtherClass::FunctionTemplatesSomeOtherClass(const FunctionTemplatesSomeOtherClass& rhs)
    {

        this->operator=(rhs);
    }

    const FunctionTemplatesSomeOtherClass& FunctionTemplatesSomeOtherClass::operator =(const FunctionTemplatesSomeOtherClass& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string FunctionTemplatesSomeOtherClass::get_type() const
    {
        return FunctionTemplatesSomeOtherClass::TYPE;
    }

    void FunctionTemplatesSomeOtherClass::serialize_xml(SerializerXml& serializer) const
    {

    }

    void FunctionTemplatesSomeOtherClass::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void FunctionTemplatesSomeOtherClass::serialize_json(SerializerJson& serializer) const
    {

    }

    void FunctionTemplatesSomeOtherClass::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
