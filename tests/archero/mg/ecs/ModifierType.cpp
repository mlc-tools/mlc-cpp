#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "ModifierType.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    ModifierType::ModifierType()
    {

    }

    ModifierType::ModifierType(int _value)
    {
        value = _value;
    }

    ModifierType::ModifierType(const ModifierType& rhs)
    {
        value = rhs.value;
    }

    ModifierType::ModifierType(const std::string& _value)
    {
        if(_value == "add")
        {
            value = add;
            return;
        }
        if(_value == "sub")
        {
            value = sub;
            return;
        }
        if(_value == "mul")
        {
            value = mul;
            return;
        }
        if(_value == "div")
        {
            value = div;
            return;
        }
        value = 0;
    }

    const ModifierType& ModifierType::operator =(const ModifierType& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const ModifierType& ModifierType::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const ModifierType& ModifierType::operator =(const std::string& _value)
    {
        if(_value == "add")
        {
            value = add;
            return *this;
        }
        if(_value == "sub")
        {
            value = sub;
            return *this;
        }
        if(_value == "mul")
        {
            value = mul;
            return *this;
        }
        if(_value == "div")
        {
            value = div;
            return *this;
        }
        return *this;
    }

    bool ModifierType::operator ==(const ModifierType& rhs) const
    {
        return value == rhs.value;
    }

    bool ModifierType::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool ModifierType::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool ModifierType::operator ==(const std::string& rhs) const
    {
        return *this == ModifierType(rhs);
    }

    bool operator ==(const std::string& lhs, const ModifierType& rhs)
    {
        return ModifierType(lhs) == rhs;
    }

    bool ModifierType::operator <(const ModifierType& rhs) const
    {
        return value < rhs.value;
    }

    ModifierType::operator int() const
    {
        return value;
    }

    ModifierType::operator std::string() const
    {
        if(value == add)
        {
            return "add";
        }
        if(value == sub)
        {
            return "sub";
        }
        if(value == mul)
        {
            return "mul";
        }
        if(value == div)
        {
            return "div";
        }
        return std::string();
    }

    std::string ModifierType::str() const
    {
        if(value == add)
        {
            return "add";
        }
        if(value == sub)
        {
            return "sub";
        }
        if(value == mul)
        {
            return "mul";
        }
        if(value == div)
        {
            return "div";
        }
        return std::string();
    }

} //namespace mg
