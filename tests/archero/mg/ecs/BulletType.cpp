#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "BulletType.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    BulletType::BulletType()
    {

    }

    BulletType::BulletType(int _value)
    {
        value = _value;
    }

    BulletType::BulletType(const BulletType& rhs)
    {
        value = rhs.value;
    }

    BulletType::BulletType(const std::string& _value)
    {
        if(_value == "arrow")
        {
            value = arrow;
            return;
        }
        if(_value == "fireball")
        {
            value = fireball;
            return;
        }
        if(_value == "balistic")
        {
            value = balistic;
            return;
        }
        if(_value == "lightning")
        {
            value = lightning;
            return;
        }
        if(_value == "laser")
        {
            value = laser;
            return;
        }
        if(_value == "meteor")
        {
            value = meteor;
            return;
        }
        if(_value == "none")
        {
            value = none;
            return;
        }
        value = 0;
    }

    const BulletType& BulletType::operator =(const BulletType& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const BulletType& BulletType::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const BulletType& BulletType::operator =(const std::string& _value)
    {
        if(_value == "arrow")
        {
            value = arrow;
            return *this;
        }
        if(_value == "fireball")
        {
            value = fireball;
            return *this;
        }
        if(_value == "balistic")
        {
            value = balistic;
            return *this;
        }
        if(_value == "lightning")
        {
            value = lightning;
            return *this;
        }
        if(_value == "laser")
        {
            value = laser;
            return *this;
        }
        if(_value == "meteor")
        {
            value = meteor;
            return *this;
        }
        if(_value == "none")
        {
            value = none;
            return *this;
        }
        return *this;
    }

    bool BulletType::operator ==(const BulletType& rhs) const
    {
        return value == rhs.value;
    }

    bool BulletType::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool BulletType::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool BulletType::operator ==(const std::string& rhs) const
    {
        return *this == BulletType(rhs);
    }

    bool operator ==(const std::string& lhs, const BulletType& rhs)
    {
        return BulletType(lhs) == rhs;
    }

    bool BulletType::operator <(const BulletType& rhs) const
    {
        return value < rhs.value;
    }

    BulletType::operator int() const
    {
        return value;
    }

    BulletType::operator std::string() const
    {
        if(value == arrow)
        {
            return "arrow";
        }
        if(value == fireball)
        {
            return "fireball";
        }
        if(value == balistic)
        {
            return "balistic";
        }
        if(value == lightning)
        {
            return "lightning";
        }
        if(value == laser)
        {
            return "laser";
        }
        if(value == meteor)
        {
            return "meteor";
        }
        if(value == none)
        {
            return "none";
        }
        return std::string();
    }

    std::string BulletType::str() const
    {
        if(value == arrow)
        {
            return "arrow";
        }
        if(value == fireball)
        {
            return "fireball";
        }
        if(value == balistic)
        {
            return "balistic";
        }
        if(value == lightning)
        {
            return "lightning";
        }
        if(value == laser)
        {
            return "laser";
        }
        if(value == meteor)
        {
            return "meteor";
        }
        if(value == none)
        {
            return "none";
        }
        return std::string();
    }

} //namespace mg
