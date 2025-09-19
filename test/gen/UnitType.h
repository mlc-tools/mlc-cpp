#ifndef __mg_UnitType_h__
#define __mg_UnitType_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BaseEnum.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
class BaseEnum;

    class UnitType : public BaseEnum
    {
    public:
        UnitType();
        
UnitType(const BaseEnum& rhs):BaseEnum(rhs){}
const UnitType& operator=(const BaseEnum& rhs){ this->value=rhs.operator int(); return *this; }
 UnitType(int _value);
 UnitType(const UnitType& rhs);
 UnitType(const std::string& _value);
const UnitType& operator =(UnitType rhs);
const UnitType& operator =(int rhs);
const UnitType& operator =(const std::string& _value);
bool operator ==(const BaseEnum& rhs) const;
bool operator ==(const UnitType& rhs) const;
bool operator ==(int rhs) const;
bool operator ==(const std::string& rhs) const;
friend bool operator ==(const std::string& lhs, const UnitType& rhs);
bool operator <(const UnitType& rhs) const;
 operator int() const;
 operator std::string() const;
std::string str() const;

static constexpr BaseEnum attack = (1);
static constexpr BaseEnum defend = (2);
static constexpr BaseEnum support = (4);

    };
} // namespace mg

#endif // __mg_UnitType_h__
