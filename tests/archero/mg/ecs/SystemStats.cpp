#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "SystemStats.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemStats::TYPE("SystemStats");

    SystemStats::SystemStats()
    : _reference_counter(1)
    {

    }

    SystemStats::~SystemStats()
    {
    }

    void SystemStats::retain()
    {
        ++this->_reference_counter;
    }

    int SystemStats::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemStats::operator ==(const SystemStats& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemStats::operator !=(const SystemStats& rhs) const
    {
        return !(*this == rhs);
    }

    SystemStats::SystemStats(const SystemStats& rhs)
    {
        this->operator=(rhs);
    }

    const SystemStats& SystemStats::operator =(const SystemStats& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemStats::get_type() const
    {
        return SystemStats::TYPE;
    }

    void SystemStats::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemStats::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemStats::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemStats::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
