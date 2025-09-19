#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestStaticPointer.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestStaticPointer::TYPE("ITestStaticPointer");

    ITestStaticPointer::ITestStaticPointer()
    {

    }

    ITestStaticPointer::~ITestStaticPointer(){}

    void ITestStaticPointer::execute()
    {
        this->test_foo();

    }

    bool ITestStaticPointer::operator ==(const ITestStaticPointer& rhs) const
    {

        bool result = true;
        return result;
    }

    bool ITestStaticPointer::operator !=(const ITestStaticPointer& rhs) const
    {

        return !(*this == rhs);
    }

    ITestStaticPointer::ITestStaticPointer(const ITestStaticPointer& rhs)
    {

        this->operator=(rhs);
    }

    const ITestStaticPointer& ITestStaticPointer::operator =(const ITestStaticPointer& rhs)
    {

        this->TestCase::operator=(rhs);
        return *this;
    }

    std::string ITestStaticPointer::get_type() const
    {
        return ITestStaticPointer::TYPE;
    }

    void ITestStaticPointer::serialize_xml(SerializerXml& serializer) const
    {

    }

    void ITestStaticPointer::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void ITestStaticPointer::serialize_json(SerializerJson& serializer) const
    {

    }

    void ITestStaticPointer::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
