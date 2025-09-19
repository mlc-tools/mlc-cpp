#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BaseEnum.h"
#include "UnitType.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{

    UnitType::UnitType()
    {

    }

    UnitType::UnitType(int _value)
    {
        value = _value;
    }

    UnitType::UnitType(const UnitType& rhs)
    {
        value = rhs.value;
    }

    UnitType::UnitType(const std::string& _value)
    {

        if(_value == "attack")
        {
            value = attack;
            return;
        }
        if(_value == "defend")
        {
            value = defend;
            return;
        }
        if(_value == "support")
        {
            value = support;
            return;
        }
        value = 0;
    }

    const UnitType& UnitType::operator =(UnitType rhs)
    {
        value = rhs.value;return *this;
    }

    const UnitType& UnitType::operator =(int rhs)
    {
        value = rhs;return *this;
    }

    const UnitType& UnitType::operator =(const std::string& _value)
    {

        if(_value == "attack")
        {
            value = attack;
            return *this;
        }
        if(_value == "defend")
        {
            value = defend;
            return *this;
        }
        if(_value == "support")
        {
            value = support;
            return *this;
        }return *this;
    }

    bool UnitType::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool UnitType::operator ==(const UnitType& rhs) const
    {
        return value == rhs.value;
    }

    bool UnitType::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool UnitType::operator ==(const std::string& rhs) const
    {
        return UnitType(rhs) == rhs;
    }

    bool operator ==(const std::string& lhs, const UnitType& rhs)
    {
        return UnitType(lhs) == rhs;
    }

    bool UnitType::operator <(const UnitType& rhs) const
    {
        return value < rhs.value;
    }

    UnitType::operator int() const
    {
        return value;
    }

    UnitType::operator std::string() const
    {

        if(value == attack)
        {
            return "attack";
        }
        if(value == defend)
        {
            return "defend";
        }
        if(value == support)
        {
            return "support";
        }return std::string();
    }

    std::string UnitType::str() const
    {

        if(value == attack)
        {
            return "attack";
        }
        if(value == defend)
        {
            return "defend";
        }
        if(value == support)
        {
            return "support";
        }return std::string();
    }

} // namespace mg
