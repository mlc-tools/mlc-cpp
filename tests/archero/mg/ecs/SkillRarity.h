#ifndef __mg_SkillRarity_h__
#define __mg_SkillRarity_h__

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

    class SkillRarity : public BaseEnum
    {
    public:
        SkillRarity();

        SkillRarity(const BaseEnum& rhs):BaseEnum(rhs){}
        const SkillRarity& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; }static SkillRarity get_random();
        SkillRarity(int _value);
        SkillRarity(const SkillRarity& rhs);
        SkillRarity(const std::string& _value);
        const SkillRarity& operator =(const SkillRarity& rhs);
        const SkillRarity& operator =(int rhs);
        const SkillRarity& operator =(const std::string& _value);
        bool operator ==(const SkillRarity& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const SkillRarity& rhs);
        bool operator <(const SkillRarity& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum fine = 0;
        static constexpr BaseEnum rare = 1;
        static constexpr BaseEnum epic = 2;
        static constexpr BaseEnum legendary = 3;

    };
} //namespace mg

#endif //#ifndef __mg_SkillRarity_h__
