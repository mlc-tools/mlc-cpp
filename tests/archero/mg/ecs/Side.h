#ifndef __mg_Side_h__
#define __mg_Side_h__

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

    class Side : public BaseEnum
    {
    public:
        Side();

        Side(const BaseEnum& rhs):BaseEnum(rhs){}
        const Side& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; }
        Side(int _value);
        Side(const Side& rhs);
        Side(const std::string& _value);
        const Side& operator =(const Side& rhs);
        const Side& operator =(int rhs);
        const Side& operator =(const std::string& _value);
        bool operator ==(const Side& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const Side& rhs);
        bool operator <(const Side& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum ally = (1);
        static constexpr BaseEnum enemy = (2);
        static constexpr BaseEnum neutral = (4);
        static constexpr BaseEnum none = (8);

    };
} //namespace mg

#endif //#ifndef __mg_Side_h__
