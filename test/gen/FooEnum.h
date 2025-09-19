#ifndef __mg_FooEnum_h__
#define __mg_FooEnum_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BaseEnum.h"
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
class BaseEnum;

    class FooEnum : public BaseEnum
    {
    public:
        FooEnum();
        
FooEnum(const BaseEnum& rhs):BaseEnum(rhs){}
const FooEnum& operator=(const BaseEnum& rhs){ this->value=rhs.operator int(); return *this; }
static std::vector<FooEnum> get_all();
 FooEnum(int _value);
 FooEnum(const FooEnum& rhs);
 FooEnum(const std::string& _value);
const FooEnum& operator =(FooEnum rhs);
const FooEnum& operator =(int rhs);
const FooEnum& operator =(const std::string& _value);
bool operator ==(const BaseEnum& rhs) const;
bool operator ==(const FooEnum& rhs) const;
bool operator ==(int rhs) const;
bool operator ==(const std::string& rhs) const;
friend bool operator ==(const std::string& lhs, const FooEnum& rhs);
bool operator <(const FooEnum& rhs) const;
 operator int() const;
 operator std::string() const;
std::string str() const;

static constexpr BaseEnum foo = (1);
static constexpr BaseEnum bar = (2);

    };
} // namespace mg

#endif // __mg_FooEnum_h__
