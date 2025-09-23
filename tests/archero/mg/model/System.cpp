#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ModelUser.h"
#include "System.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string System::TYPE("System");

    System::System()
    : _reference_counter(1)
    {

    }

    System::~System()
    {
    }

    void System::retain()
    {
        ++this->_reference_counter;
    }

    int System::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool System::operator ==(const System& rhs) const
    {
        bool result = true;
        return result;
    }

    bool System::operator !=(const System& rhs) const
    {
        return !(*this == rhs);
    }

    System::System(const System& rhs)
    {
        this->operator=(rhs);
    }

    const System& System::operator =(const System& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string System::get_type() const
    {
        return System::TYPE;
    }

    void System::serialize_xml(SerializerXml& serializer) const
    {

    }

    void System::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void System::serialize_json(SerializerJson& serializer) const
    {

    }

    void System::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
