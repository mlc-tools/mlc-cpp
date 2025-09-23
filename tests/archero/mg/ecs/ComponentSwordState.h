#ifndef __mg_ComponentSwordState_h__
#define __mg_ComponentSwordState_h__

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

    class ComponentSwordState : public BaseEnum
    {
    public:
        ComponentSwordState();

        ComponentSwordState(const BaseEnum& rhs):BaseEnum(rhs){}
        const ComponentSwordState& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; }
        ComponentSwordState(int _value);
        ComponentSwordState(const ComponentSwordState& rhs);
        ComponentSwordState(const std::string& _value);
        const ComponentSwordState& operator =(const ComponentSwordState& rhs);
        const ComponentSwordState& operator =(int rhs);
        const ComponentSwordState& operator =(const std::string& _value);
        bool operator ==(const ComponentSwordState& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const ComponentSwordState& rhs);
        bool operator <(const ComponentSwordState& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum appearance = (1);
        static constexpr BaseEnum prepare_to_fly = (2);
        static constexpr BaseEnum fly = (4);

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSwordState_h__
