#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BaseEnum.h"
#include "Resource.h"
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{

    Resource::Resource()
    {

    }

    Resource::Resource(int _value)
    {
        value = _value;
    }

    Resource::Resource(const Resource& rhs)
    {
        value = rhs.value;
    }

    Resource::Resource(const std::string& _value)
    {

        if(_value == "gold")
        {
            value = gold;
            return;
        }
        if(_value == "gems")
        {
            value = gems;
            return;
        }
        value = 0;
    }

    const Resource& Resource::operator =(Resource rhs)
    {
        value = rhs.value;return *this;
    }

    const Resource& Resource::operator =(int rhs)
    {
        value = rhs;return *this;
    }

    const Resource& Resource::operator =(const std::string& _value)
    {

        if(_value == "gold")
        {
            value = gold;
            return *this;
        }
        if(_value == "gems")
        {
            value = gems;
            return *this;
        }return *this;
    }

    bool Resource::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool Resource::operator ==(const Resource& rhs) const
    {
        return value == rhs.value;
    }

    bool Resource::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool Resource::operator ==(const std::string& rhs) const
    {
        return Resource(rhs) == rhs;
    }

    bool operator ==(const std::string& lhs, const Resource& rhs)
    {
        return Resource(lhs) == rhs;
    }

    bool Resource::operator <(const Resource& rhs) const
    {
        return value < rhs.value;
    }

    Resource::operator int() const
    {
        return value;
    }

    Resource::operator std::string() const
    {

        if(value == gold)
        {
            return "gold";
        }
        if(value == gems)
        {
            return "gems";
        }return std::string();
    }

    std::string Resource::str() const
    {

        if(value == gold)
        {
            return "gold";
        }
        if(value == gems)
        {
            return "gems";
        }return std::string();
    }

} // namespace mg
