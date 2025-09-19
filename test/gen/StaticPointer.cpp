#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "StaticPointer.h"
#include "core/CommandBase.h"
#include <atomic>
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    intrusive_ptr<CommandBase> StaticPointer::command(nullptr);
    const std::string StaticPointer::TYPE("StaticPointer");

    StaticPointer::StaticPointer()
    : _reference_counter(1)
    {

    }

    StaticPointer::~StaticPointer(){}

    void StaticPointer::retain()
    {
        ++this->_reference_counter;
    }

    int StaticPointer::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool StaticPointer::operator ==(const StaticPointer& rhs) const
    {
        bool result = true;
        return result;
    }

    bool StaticPointer::operator !=(const StaticPointer& rhs) const
    {

        return !(*this == rhs);
    }

    StaticPointer::StaticPointer(const StaticPointer& rhs)
    {

        this->operator=(rhs);
    }

    const StaticPointer& StaticPointer::operator =(const StaticPointer& rhs)
    {

        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string StaticPointer::get_type() const
    {
        return StaticPointer::TYPE;
    }

    void StaticPointer::serialize_xml(SerializerXml& serializer) const
    {

    }

    void StaticPointer::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void StaticPointer::serialize_json(SerializerJson& serializer) const
    {

    }

    void StaticPointer::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
