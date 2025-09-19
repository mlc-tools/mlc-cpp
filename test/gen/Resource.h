#ifndef __mg_Resource_h__
#define __mg_Resource_h__

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

    class Resource : public BaseEnum
    {
    public:
        Resource();

        Resource(const BaseEnum& rhs):BaseEnum(rhs){}
        const Resource& operator=(const BaseEnum& rhs){ this->value=rhs.operator int(); return *this; }
        Resource(int _value);
        Resource(const Resource& rhs);
        Resource(const std::string& _value);
        const Resource& operator =(Resource rhs);
        const Resource& operator =(int rhs);
        const Resource& operator =(const std::string& _value);
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(const Resource& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const Resource& rhs);
        bool operator <(const Resource& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum gold = (1);
        static constexpr BaseEnum gems = (2);

    };
} // namespace mg

#endif // __mg_Resource_h__
