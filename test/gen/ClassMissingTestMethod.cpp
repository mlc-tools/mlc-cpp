#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "ClassMissingTestMethod.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string ClassMissingTestMethod::TYPE("ClassMissingTestMethod");

    ClassMissingTestMethod::ClassMissingTestMethod()
    : _reference_counter(1)
    {

    }

    ClassMissingTestMethod::~ClassMissingTestMethod(){}

    void ClassMissingTestMethod::foo()
    {

    }

    void ClassMissingTestMethod::bar()
    {

    }

    void ClassMissingTestMethod::retain()
    {
        ++this->_reference_counter;
    }

    int ClassMissingTestMethod::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool ClassMissingTestMethod::operator ==(const ClassMissingTestMethod& rhs) const
    {
        bool result = true;
        return result;
    }

    bool ClassMissingTestMethod::operator !=(const ClassMissingTestMethod& rhs) const
    {

        return !(*this == rhs);
    }

    ClassMissingTestMethod::ClassMissingTestMethod(const ClassMissingTestMethod& rhs)
    {

        this->operator=(rhs);
    }

    const ClassMissingTestMethod& ClassMissingTestMethod::operator =(const ClassMissingTestMethod& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ClassMissingTestMethod::get_type() const
    {
        return ClassMissingTestMethod::TYPE;
    }

    void ClassMissingTestMethod::serialize_xml(SerializerXml& serializer) const
    {

    }

    void ClassMissingTestMethod::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void ClassMissingTestMethod::serialize_json(SerializerJson& serializer) const
    {

    }

    void ClassMissingTestMethod::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
