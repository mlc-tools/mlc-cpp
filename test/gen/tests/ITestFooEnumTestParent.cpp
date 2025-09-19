#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestFooEnumTestParent.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestFooEnumTestParent::TYPE("ITestFooEnumTestParent");

    ITestFooEnumTestParent::ITestFooEnumTestParent()
    {

    }

    ITestFooEnumTestParent::~ITestFooEnumTestParent(){}

    void ITestFooEnumTestParent::execute()
    {
        this->test_dummy();
        this->test_compare();
        this->test_initialize();
        this->test_enum_with_int_values();

    }

    bool ITestFooEnumTestParent::operator ==(const ITestFooEnumTestParent& rhs) const
    {

        bool result = true;
        return result;
    }

    bool ITestFooEnumTestParent::operator !=(const ITestFooEnumTestParent& rhs) const
    {

        return !(*this == rhs);
    }

    ITestFooEnumTestParent::ITestFooEnumTestParent(const ITestFooEnumTestParent& rhs)
    {

        this->operator=(rhs);
    }

    const ITestFooEnumTestParent& ITestFooEnumTestParent::operator =(const ITestFooEnumTestParent& rhs)
    {

        this->TestCase::operator=(rhs);
        return *this;
    }

    std::string ITestFooEnumTestParent::get_type() const
    {
        return ITestFooEnumTestParent::TYPE;
    }

    void ITestFooEnumTestParent::serialize_xml(SerializerXml& serializer) const
    {

    }

    void ITestFooEnumTestParent::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void ITestFooEnumTestParent::serialize_json(SerializerJson& serializer) const
    {

    }

    void ITestFooEnumTestParent::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
