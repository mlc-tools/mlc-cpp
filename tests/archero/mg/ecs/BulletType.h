#ifndef __mg_BulletType_h__
#define __mg_BulletType_h__

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

    class BulletType : public BaseEnum
    {
    public:
        BulletType();

        BulletType(const BaseEnum& rhs):BaseEnum(rhs){}
        const BulletType& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; } BulletType(int _value);
        BulletType(const BulletType& rhs);
        BulletType(const std::string& _value);
        const BulletType& operator =(const BulletType& rhs);
        const BulletType& operator =(int rhs);
        const BulletType& operator =(const std::string& _value);
        bool operator ==(const BulletType& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const BulletType& rhs);
        bool operator <(const BulletType& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum arrow = (1 << 0);
        static constexpr BaseEnum fireball = (1 << 1);
        static constexpr BaseEnum balistic = (1 << 2);
        static constexpr BaseEnum lightning = (1 << 3);
        static constexpr BaseEnum laser = (1 << 4);
        static constexpr BaseEnum meteor = (1 << 5);
        static constexpr BaseEnum none = (1 << 6);

    };
} //namespace mg

#endif //#ifndef __mg_BulletType_h__
