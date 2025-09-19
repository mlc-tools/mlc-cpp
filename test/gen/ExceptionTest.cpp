#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "ExceptionTest.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string ExceptionTest::TYPE("ExceptionTest");

    ExceptionTest::ExceptionTest()
    : _reference_counter(1)
    {

    }

    ExceptionTest::~ExceptionTest(){}

    bool ExceptionTest::test()
    {

        int k = -1;
        try
        {
            k=0;
            throw std::exception();;
        }
        catch(const std::exception& e)
        {
            k=1;
        }
        return k == 1;

    }

    void ExceptionTest::retain()
    {
        ++this->_reference_counter;
    }

    int ExceptionTest::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool ExceptionTest::operator ==(const ExceptionTest& rhs) const
    {
        bool result = true;
        return result;
    }

    bool ExceptionTest::operator !=(const ExceptionTest& rhs) const
    {

        return !(*this == rhs);
    }

    ExceptionTest::ExceptionTest(const ExceptionTest& rhs)
    {

        this->operator=(rhs);
    }

    const ExceptionTest& ExceptionTest::operator =(const ExceptionTest& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ExceptionTest::get_type() const
    {
        return ExceptionTest::TYPE;
    }

    void ExceptionTest::serialize_xml(SerializerXml& serializer) const
    {

    }

    void ExceptionTest::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void ExceptionTest::serialize_json(SerializerJson& serializer) const
    {

    }

    void ExceptionTest::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
