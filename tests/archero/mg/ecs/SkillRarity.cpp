#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "SkillRarity.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    SkillRarity::SkillRarity()
    {

    }


    SkillRarity SkillRarity::get_random()
    {
        int r = random_int(0, SkillRarity::legendary + 1);
        if(r >= SkillRarity::legendary) return SkillRarity::legendary;
        if(r >= SkillRarity::epic) return SkillRarity::legendary;
        if(r >= SkillRarity::rare) return SkillRarity::rare;
        return SkillRarity::fine;
    }

    SkillRarity::SkillRarity(int _value)
    {
        value = _value;
    }

    SkillRarity::SkillRarity(const SkillRarity& rhs)
    {
        value = rhs.value;
    }

    SkillRarity::SkillRarity(const std::string& _value)
    {
        if(_value == "fine")
        {
            value = fine;
            return;
        }
        if(_value == "rare")
        {
            value = rare;
            return;
        }
        if(_value == "epic")
        {
            value = epic;
            return;
        }
        if(_value == "legendary")
        {
            value = legendary;
            return;
        }
        value = 0;
    }

    const SkillRarity& SkillRarity::operator =(const SkillRarity& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const SkillRarity& SkillRarity::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const SkillRarity& SkillRarity::operator =(const std::string& _value)
    {
        if(_value == "fine")
        {
            value = fine;
            return *this;
        }
        if(_value == "rare")
        {
            value = rare;
            return *this;
        }
        if(_value == "epic")
        {
            value = epic;
            return *this;
        }
        if(_value == "legendary")
        {
            value = legendary;
            return *this;
        }
        return *this;
    }

    bool SkillRarity::operator ==(const SkillRarity& rhs) const
    {
        return value == rhs.value;
    }

    bool SkillRarity::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool SkillRarity::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool SkillRarity::operator ==(const std::string& rhs) const
    {
        return *this == SkillRarity(rhs);
    }

    bool operator ==(const std::string& lhs, const SkillRarity& rhs)
    {
        return SkillRarity(lhs) == rhs;
    }

    bool SkillRarity::operator <(const SkillRarity& rhs) const
    {
        return value < rhs.value;
    }

    SkillRarity::operator int() const
    {
        return value;
    }

    SkillRarity::operator std::string() const
    {
        if(value == fine)
        {
            return "fine";
        }
        if(value == rare)
        {
            return "rare";
        }
        if(value == epic)
        {
            return "epic";
        }
        if(value == legendary)
        {
            return "legendary";
        }
        return std::string();
    }

    std::string SkillRarity::str() const
    {
        if(value == fine)
        {
            return "fine";
        }
        if(value == rare)
        {
            return "rare";
        }
        if(value == epic)
        {
            return "epic";
        }
        if(value == legendary)
        {
            return "legendary";
        }
        return std::string();
    }

} //namespace mg
