#ifndef __mg_TestEnum_h__
#define __mg_TestEnum_h__

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

    class TestEnum : public BaseEnum
    {
    public:
        TestEnum();
        
TestEnum(const BaseEnum& rhs):BaseEnum(rhs){}
const TestEnum& operator=(const BaseEnum& rhs){ this->value=rhs.operator int(); return *this; }
 TestEnum(int _value);
 TestEnum(const TestEnum& rhs);
 TestEnum(const std::string& _value);
const TestEnum& operator =(TestEnum rhs);
const TestEnum& operator =(int rhs);
const TestEnum& operator =(const std::string& _value);
bool operator ==(const BaseEnum& rhs) const;
bool operator ==(const TestEnum& rhs) const;
bool operator ==(int rhs) const;
bool operator ==(const std::string& rhs) const;
friend bool operator ==(const std::string& lhs, const TestEnum& rhs);
bool operator <(const TestEnum& rhs) const;
 operator int() const;
 operator std::string() const;
std::string str() const;

static constexpr BaseEnum value1 = 0;
static constexpr BaseEnum value2 = 1;

    };
} // namespace mg

#endif // __mg_TestEnum_h__
