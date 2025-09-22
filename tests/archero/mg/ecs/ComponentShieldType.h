#ifndef __mg_ComponentShieldType_h__
#define __mg_ComponentShieldType_h__

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

    class ComponentShieldType : public BaseEnum
    {
    public:
        ComponentShieldType();

        ComponentShieldType(const BaseEnum& rhs):BaseEnum(rhs){}
        const ComponentShieldType& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; } ComponentShieldType(int _value);
        ComponentShieldType(const ComponentShieldType& rhs);
        ComponentShieldType(const std::string& _value);
        const ComponentShieldType& operator =(const ComponentShieldType& rhs);
        const ComponentShieldType& operator =(int rhs);
        const ComponentShieldType& operator =(const std::string& _value);
        bool operator ==(const ComponentShieldType& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const ComponentShieldType& rhs);
        bool operator <(const ComponentShieldType& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum wave = (1 << 0);
        static constexpr BaseEnum timer = (1 << 1);

    };
} //namespace mg

#endif //#ifndef __mg_ComponentShieldType_h__
