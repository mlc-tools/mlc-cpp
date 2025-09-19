#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestFunctionTemplates.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestFunctionTemplates::TYPE("ITestFunctionTemplates");

    ITestFunctionTemplates::ITestFunctionTemplates()
    {

    }

    ITestFunctionTemplates::~ITestFunctionTemplates(){}

    void ITestFunctionTemplates::execute()
    {
        this->test_add();
        this->test_use_other_class();
        this->test_add_strings();

    }

    bool ITestFunctionTemplates::operator ==(const ITestFunctionTemplates& rhs) const
    {

        bool result = true;
        return result;
    }

    bool ITestFunctionTemplates::operator !=(const ITestFunctionTemplates& rhs) const
    {

        return !(*this == rhs);
    }

    ITestFunctionTemplates::ITestFunctionTemplates(const ITestFunctionTemplates& rhs)
    {

        this->operator=(rhs);
    }

    const ITestFunctionTemplates& ITestFunctionTemplates::operator =(const ITestFunctionTemplates& rhs)
    {

        this->TestCase::operator=(rhs);
        return *this;
    }

    std::string ITestFunctionTemplates::get_type() const
    {
        return ITestFunctionTemplates::TYPE;
    }

    void ITestFunctionTemplates::serialize_xml(SerializerXml& serializer) const
    {

    }

    void ITestFunctionTemplates::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void ITestFunctionTemplates::serialize_json(SerializerJson& serializer) const
    {

    }

    void ITestFunctionTemplates::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
