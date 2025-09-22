#ifndef __mg_ModifierType_h__
#define __mg_ModifierType_h__

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

    class ModifierType : public BaseEnum
    {
    public:
        ModifierType();

        ModifierType(const BaseEnum& rhs):BaseEnum(rhs){}
        const ModifierType& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; } ModifierType(int _value);
        ModifierType(const ModifierType& rhs);
        ModifierType(const std::string& _value);
        const ModifierType& operator =(const ModifierType& rhs);
        const ModifierType& operator =(int rhs);
        const ModifierType& operator =(const std::string& _value);
        bool operator ==(const ModifierType& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const ModifierType& rhs);
        bool operator <(const ModifierType& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum add = (1 << 0);
        static constexpr BaseEnum sub = (1 << 1);
        static constexpr BaseEnum mul = (1 << 2);
        static constexpr BaseEnum div = (1 << 3);

    };
} //namespace mg

#endif //#ifndef __mg_ModifierType_h__
