#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BaseEnum.h"
#include "EnumWithIntValue.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{

    EnumWithIntValue::EnumWithIntValue()
    {

    }

    EnumWithIntValue::EnumWithIntValue(int _value)
    {
        value = _value;
    }

    EnumWithIntValue::EnumWithIntValue(const EnumWithIntValue& rhs)
    {
        value = rhs.value;
    }

    EnumWithIntValue::EnumWithIntValue(const std::string& _value)
    {

        if(_value == "foo")
        {
            value = foo;
            return;
        }
        if(_value == "bar")
        {
            value = bar;
            return;
        }
        if(_value == "foo_bar")
        {
            value = foo_bar;
            return;
        }
        value = 0;
    }

    const EnumWithIntValue& EnumWithIntValue::operator =(EnumWithIntValue rhs)
    {
        value = rhs.value;return *this;
    }

    const EnumWithIntValue& EnumWithIntValue::operator =(int rhs)
    {
        value = rhs;return *this;
    }

    const EnumWithIntValue& EnumWithIntValue::operator =(const std::string& _value)
    {

        if(_value == "foo")
        {
            value = foo;
            return *this;
        }
        if(_value == "bar")
        {
            value = bar;
            return *this;
        }
        if(_value == "foo_bar")
        {
            value = foo_bar;
            return *this;
        }return *this;
    }

    bool EnumWithIntValue::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool EnumWithIntValue::operator ==(const EnumWithIntValue& rhs) const
    {
        return value == rhs.value;
    }

    bool EnumWithIntValue::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool EnumWithIntValue::operator ==(const std::string& rhs) const
    {
        return EnumWithIntValue(rhs) == rhs;
    }

    bool operator ==(const std::string& lhs, const EnumWithIntValue& rhs)
    {
        return EnumWithIntValue(lhs) == rhs;
    }

    bool EnumWithIntValue::operator <(const EnumWithIntValue& rhs) const
    {
        return value < rhs.value;
    }

    EnumWithIntValue::operator int() const
    {
        return value;
    }

    EnumWithIntValue::operator std::string() const
    {

        if(value == foo)
        {
            return "foo";
        }
        if(value == bar)
        {
            return "bar";
        }
        if(value == foo_bar)
        {
            return "foo_bar";
        }return std::string();
    }

    std::string EnumWithIntValue::str() const
    {

        if(value == foo)
        {
            return "foo";
        }
        if(value == bar)
        {
            return "bar";
        }
        if(value == foo_bar)
        {
            return "foo_bar";
        }return std::string();
    }

} // namespace mg
