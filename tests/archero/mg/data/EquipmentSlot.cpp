#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "EquipmentSlot.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    EquipmentSlot::EquipmentSlot()
    {

    }


    EquipmentSlot::EquipmentSlot(int _value)
    {
        value = _value;
    }

    EquipmentSlot::EquipmentSlot(const EquipmentSlot& rhs)
    {
        value = rhs.value;
    }

    EquipmentSlot::EquipmentSlot(const std::string& _value)
    {
        if(_value == "weapon")
        {
            value = weapon;
            return;
        }
        if(_value == "ring")
        {
            value = ring;
            return;
        }
        if(_value == "amulet")
        {
            value = amulet;
            return;
        }
        if(_value == "armor")
        {
            value = armor;
            return;
        }
        if(_value == "head")
        {
            value = head;
            return;
        }
        if(_value == "boots")
        {
            value = boots;
            return;
        }
        value = 0;
    }

    const EquipmentSlot& EquipmentSlot::operator =(const EquipmentSlot& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const EquipmentSlot& EquipmentSlot::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const EquipmentSlot& EquipmentSlot::operator =(const std::string& _value)
    {
        if(_value == "weapon")
        {
            value = weapon;
            return *this;
        }
        if(_value == "ring")
        {
            value = ring;
            return *this;
        }
        if(_value == "amulet")
        {
            value = amulet;
            return *this;
        }
        if(_value == "armor")
        {
            value = armor;
            return *this;
        }
        if(_value == "head")
        {
            value = head;
            return *this;
        }
        if(_value == "boots")
        {
            value = boots;
            return *this;
        }
        return *this;
    }

    bool EquipmentSlot::operator ==(const EquipmentSlot& rhs) const
    {
        return value == rhs.value;
    }

    bool EquipmentSlot::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool EquipmentSlot::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool EquipmentSlot::operator ==(const std::string& rhs) const
    {
        return *this == EquipmentSlot(rhs);
    }

    bool operator ==(const std::string& lhs, const EquipmentSlot& rhs)
    {
        return EquipmentSlot(lhs) == rhs;
    }

    bool EquipmentSlot::operator <(const EquipmentSlot& rhs) const
    {
        return value < rhs.value;
    }

    EquipmentSlot::operator int() const
    {
        return value;
    }

    EquipmentSlot::operator std::string() const
    {
        if(value == weapon)
        {
            return "weapon";
        }
        if(value == ring)
        {
            return "ring";
        }
        if(value == amulet)
        {
            return "amulet";
        }
        if(value == armor)
        {
            return "armor";
        }
        if(value == head)
        {
            return "head";
        }
        if(value == boots)
        {
            return "boots";
        }
        return std::string();
    }

    std::string EquipmentSlot::str() const
    {
        if(value == weapon)
        {
            return "weapon";
        }
        if(value == ring)
        {
            return "ring";
        }
        if(value == amulet)
        {
            return "amulet";
        }
        if(value == armor)
        {
            return "armor";
        }
        if(value == head)
        {
            return "head";
        }
        if(value == boots)
        {
            return "boots";
        }
        return std::string();
    }

} //namespace mg
