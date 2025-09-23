#ifndef __mg_EquipmentSlot_h__
#define __mg_EquipmentSlot_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../BaseEnum.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class BaseEnum;

    class EquipmentSlot : public BaseEnum
    {
    public:
        EquipmentSlot();

        EquipmentSlot(const BaseEnum& rhs):BaseEnum(rhs){}
        const EquipmentSlot& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; }
        EquipmentSlot(int _value);
        EquipmentSlot(const EquipmentSlot& rhs);
        EquipmentSlot(const std::string& _value);
        const EquipmentSlot& operator =(const EquipmentSlot& rhs);
        const EquipmentSlot& operator =(int rhs);
        const EquipmentSlot& operator =(const std::string& _value);
        bool operator ==(const EquipmentSlot& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const EquipmentSlot& rhs);
        bool operator <(const EquipmentSlot& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum weapon = (1);
        static constexpr BaseEnum ring = (2);
        static constexpr BaseEnum amulet = (4);
        static constexpr BaseEnum armor = (8);
        static constexpr BaseEnum head = (16);
        static constexpr BaseEnum boots = (32);

    };
} //namespace mg

#endif //#ifndef __mg_EquipmentSlot_h__
