#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ITestDataListListBool.h"
#include "TestCase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ITestDataListListBool::TYPE("ITestDataListListBool");

    ITestDataListListBool::ITestDataListListBool()
    {

    }

    ITestDataListListBool::~ITestDataListListBool(){}

    void ITestDataListListBool::execute()
    {
        this->test_foo();

    }

    bool ITestDataListListBool::operator ==(const ITestDataListListBool& rhs) const
    {

        bool result = true;
        return result;
    }

    bool ITestDataListListBool::operator !=(const ITestDataListListBool& rhs) const
    {

        return !(*this == rhs);
    }

    ITestDataListListBool::ITestDataListListBool(const ITestDataListListBool& rhs)
    {

        this->operator=(rhs);
    }

    const ITestDataListListBool& ITestDataListListBool::operator =(const ITestDataListListBool& rhs)
    {

        this->TestCase::operator=(rhs);
        return *this;
    }

    std::string ITestDataListListBool::get_type() const
    {
        return ITestDataListListBool::TYPE;
    }

    void ITestDataListListBool::serialize_xml(SerializerXml& serializer) const
    {

    }

    void ITestDataListListBool::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void ITestDataListListBool::serialize_json(SerializerJson& serializer) const
    {

    }

    void ITestDataListListBool::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
