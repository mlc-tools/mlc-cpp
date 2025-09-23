#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "ComponentShieldType.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    ComponentShieldType::ComponentShieldType()
    {

    }

    ComponentShieldType::ComponentShieldType(int _value)
    {
        value = _value;
    }

    ComponentShieldType::ComponentShieldType(const ComponentShieldType& rhs)
    {
        value = rhs.value;
    }

    ComponentShieldType::ComponentShieldType(const std::string& _value)
    {
        if(_value == "wave")
        {
            value = wave;
            return;
        }
        if(_value == "timer")
        {
            value = timer;
            return;
        }
        value = 0;
    }

    const ComponentShieldType& ComponentShieldType::operator =(const ComponentShieldType& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const ComponentShieldType& ComponentShieldType::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const ComponentShieldType& ComponentShieldType::operator =(const std::string& _value)
    {
        if(_value == "wave")
        {
            value = wave;
            return *this;
        }
        if(_value == "timer")
        {
            value = timer;
            return *this;
        }
        return *this;
    }

    bool ComponentShieldType::operator ==(const ComponentShieldType& rhs) const
    {
        return value == rhs.value;
    }

    bool ComponentShieldType::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool ComponentShieldType::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool ComponentShieldType::operator ==(const std::string& rhs) const
    {
        return *this == ComponentShieldType(rhs);
    }

    bool operator ==(const std::string& lhs, const ComponentShieldType& rhs)
    {
        return ComponentShieldType(lhs) == rhs;
    }

    bool ComponentShieldType::operator <(const ComponentShieldType& rhs) const
    {
        return value < rhs.value;
    }

    ComponentShieldType::operator int() const
    {
        return value;
    }

    ComponentShieldType::operator std::string() const
    {
        if(value == wave)
        {
            return "wave";
        }
        if(value == timer)
        {
            return "timer";
        }
        return std::string();
    }

    std::string ComponentShieldType::str() const
    {
        if(value == wave)
        {
            return "wave";
        }
        if(value == timer)
        {
            return "timer";
        }
        return std::string();
    }

} //namespace mg
