#ifndef __mg_EnumWithIntValue_h__
#define __mg_EnumWithIntValue_h__

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

    class EnumWithIntValue : public BaseEnum
    {
    public:
        EnumWithIntValue();
        
EnumWithIntValue(const BaseEnum& rhs):BaseEnum(rhs){}
const EnumWithIntValue& operator=(const BaseEnum& rhs){ this->value=rhs.operator int(); return *this; }
 EnumWithIntValue(int _value);
 EnumWithIntValue(const EnumWithIntValue& rhs);
 EnumWithIntValue(const std::string& _value);
const EnumWithIntValue& operator =(EnumWithIntValue rhs);
const EnumWithIntValue& operator =(int rhs);
const EnumWithIntValue& operator =(const std::string& _value);
bool operator ==(const BaseEnum& rhs) const;
bool operator ==(const EnumWithIntValue& rhs) const;
bool operator ==(int rhs) const;
bool operator ==(const std::string& rhs) const;
friend bool operator ==(const std::string& lhs, const EnumWithIntValue& rhs);
bool operator <(const EnumWithIntValue& rhs) const;
 operator int() const;
 operator std::string() const;
std::string str() const;

static constexpr BaseEnum foo = 0x1;
static constexpr BaseEnum bar = 0x2;
static constexpr BaseEnum foo_bar = 0x3;

    };
} // namespace mg

#endif // __mg_EnumWithIntValue_h__
