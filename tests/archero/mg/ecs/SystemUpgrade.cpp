#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ModelEcsBase.h"
#include "SystemUpgrade.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemUpgrade::TYPE("SystemUpgrade");

    SystemUpgrade::SystemUpgrade()
    : _reference_counter(1)
    {

    }

    SystemUpgrade::~SystemUpgrade()
    {
    }

    void SystemUpgrade::update(ModelEcsBase* model, float dt)
    {

    }

    void SystemUpgrade::retain()
    {
        ++this->_reference_counter;
    }

    int SystemUpgrade::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemUpgrade::operator ==(const SystemUpgrade& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemUpgrade::operator !=(const SystemUpgrade& rhs) const
    {
        return !(*this == rhs);
    }

    SystemUpgrade::SystemUpgrade(const SystemUpgrade& rhs)
    {
        this->operator=(rhs);
    }

    const SystemUpgrade& SystemUpgrade::operator =(const SystemUpgrade& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemUpgrade::get_type() const
    {
        return SystemUpgrade::TYPE;
    }

    void SystemUpgrade::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemUpgrade::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemUpgrade::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemUpgrade::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
