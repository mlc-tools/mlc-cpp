#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CompareTestBase.h"
#include "CompareTestWithoutPointer.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CompareTestWithoutPointer::TYPE("CompareTestWithoutPointer");

    CompareTestWithoutPointer::CompareTestWithoutPointer()
    : a(0)
    , b()
    {

    }

    CompareTestWithoutPointer::~CompareTestWithoutPointer(){}

    void CompareTestWithoutPointer::test()
    {

    }

    bool CompareTestWithoutPointer::operator ==(const CompareTestWithoutPointer& rhs) const
    {

        bool result = this->CompareTestBase::operator ==(rhs);
        result = result && this->a == rhs.a;
        result = result && this->b == rhs.b;
        return result;
    }

    bool CompareTestWithoutPointer::operator !=(const CompareTestWithoutPointer& rhs) const
    {

        return !(*this == rhs);
    }

    CompareTestWithoutPointer::CompareTestWithoutPointer(const CompareTestWithoutPointer& rhs)
    {

        this->operator=(rhs);
    }

    const CompareTestWithoutPointer& CompareTestWithoutPointer::operator =(const CompareTestWithoutPointer& rhs)
    {

        this->CompareTestBase::operator=(rhs);
        this->a = rhs.a;
        this->b = rhs.b;
        return *this;
    }

    std::string CompareTestWithoutPointer::get_type() const
    {
        return CompareTestWithoutPointer::TYPE;
    }

    void CompareTestWithoutPointer::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(a, "a", int(0));
        serializer.serialize(b, "b", std::string());
    }

    void CompareTestWithoutPointer::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(a, "a", int(0));
        deserializer.deserialize(b, "b", std::string());
    }

    void CompareTestWithoutPointer::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(a, "a", int(0));
        serializer.serialize(b, "b", std::string());
    }

    void CompareTestWithoutPointer::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(a, "a", int(0));
        deserializer.deserialize(b, "b", std::string());
    }

} // namespace mg
