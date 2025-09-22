#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "Side.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    Side::Side()
    {

    }


    Side::Side(int _value)
    {
        value = _value;
    }

    Side::Side(const Side& rhs)
    {
        value = rhs.value;
    }

    Side::Side(const std::string& _value)
    {
        if(_value == "ally")
        {
            value = ally;
            return;
        }
        if(_value == "enemy")
        {
            value = enemy;
            return;
        }
        if(_value == "neutral")
        {
            value = neutral;
            return;
        }
        if(_value == "none")
        {
            value = none;
            return;
        }
        value = 0;
    }

    const Side& Side::operator =(const Side& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const Side& Side::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const Side& Side::operator =(const std::string& _value)
    {
        if(_value == "ally")
        {
            value = ally;
            return *this;
        }
        if(_value == "enemy")
        {
            value = enemy;
            return *this;
        }
        if(_value == "neutral")
        {
            value = neutral;
            return *this;
        }
        if(_value == "none")
        {
            value = none;
            return *this;
        }
        return *this;
    }

    bool Side::operator ==(const Side& rhs) const
    {
        return value == rhs.value;
    }

    bool Side::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool Side::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool Side::operator ==(const std::string& rhs) const
    {
        return *this == Side(rhs);
    }

    bool operator ==(const std::string& lhs, const Side& rhs)
    {
        return Side(lhs) == rhs;
    }

    bool Side::operator <(const Side& rhs) const
    {
        return value < rhs.value;
    }

    Side::operator int() const
    {
        return value;
    }

    Side::operator std::string() const
    {
        if(value == ally)
        {
            return "ally";
        }
        if(value == enemy)
        {
            return "enemy";
        }
        if(value == neutral)
        {
            return "neutral";
        }
        if(value == none)
        {
            return "none";
        }
        return std::string();
    }

    std::string Side::str() const
    {
        if(value == ally)
        {
            return "ally";
        }
        if(value == enemy)
        {
            return "enemy";
        }
        if(value == neutral)
        {
            return "neutral";
        }
        if(value == none)
        {
            return "none";
        }
        return std::string();
    }

} //namespace mg
