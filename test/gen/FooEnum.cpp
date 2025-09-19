#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "BaseEnum.h"
#include "FooEnum.h"
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    
    
    FooEnum::FooEnum()
    {
    
    }

    

std::vector<FooEnum> FooEnum::get_all()
{

		std::vector<FooEnum> stats;
		list_push(stats, FooEnum::foo);
		list_push(stats, FooEnum::bar);
		return stats;
	
}

 FooEnum::FooEnum(int _value)
{
value = _value;
}

 FooEnum::FooEnum(const FooEnum& rhs)
{
value = rhs.value;
}

 FooEnum::FooEnum(const std::string& _value)
{

if(_value == "foo") 
{ 
value = foo; 
return; 
}
if(_value == "bar") 
{ 
value = bar; 
return; 
}
value = 0;
}

const FooEnum& FooEnum::operator =(FooEnum rhs)
{
value = rhs.value;return *this;
}

const FooEnum& FooEnum::operator =(int rhs)
{
value = rhs;return *this;
}

const FooEnum& FooEnum::operator =(const std::string& _value)
{

if(_value == "foo") 
{
 value = foo; 
return *this; 
}
if(_value == "bar") 
{
 value = bar; 
return *this; 
}return *this;
}

bool FooEnum::operator ==(const BaseEnum& rhs) const
{
return value == rhs.operator int();
}

bool FooEnum::operator ==(const FooEnum& rhs) const
{
return value == rhs.value;
}

bool FooEnum::operator ==(int rhs) const
{
return value == rhs;
}

bool FooEnum::operator ==(const std::string& rhs) const
{
return FooEnum(rhs) == rhs;
}

bool operator ==(const std::string& lhs, const FooEnum& rhs)
{
return FooEnum(lhs) == rhs;
}

bool FooEnum::operator <(const FooEnum& rhs) const
{
return value < rhs.value;
}

 FooEnum::operator int() const
{
return value;
}

 FooEnum::operator std::string() const
{

if(value == foo) 
{
 return "foo"; 
}
if(value == bar) 
{
 return "bar"; 
}return std::string();
}

std::string FooEnum::str() const
{

if(value == foo) 
{
 return "foo"; 
}
if(value == bar) 
{
 return "bar"; 
}return std::string();
}

} // namespace mg
