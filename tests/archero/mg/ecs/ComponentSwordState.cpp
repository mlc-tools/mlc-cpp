#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "ComponentSword.h"
#include "ComponentSwordState.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    ComponentSwordState::ComponentSwordState()
    {

    }


    ComponentSwordState::ComponentSwordState(int _value)
    {
        value = _value;
    }

    ComponentSwordState::ComponentSwordState(const ComponentSwordState& rhs)
    {
        value = rhs.value;
    }

    ComponentSwordState::ComponentSwordState(const std::string& _value)
    {
        if(_value == "appearance")
        {
            value = appearance;
            return;
        }
        if(_value == "prepare_to_fly")
        {
            value = prepare_to_fly;
            return;
        }
        if(_value == "fly")
        {
            value = fly;
            return;
        }
        value = 0;
    }

    const ComponentSwordState& ComponentSwordState::operator =(const ComponentSwordState& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const ComponentSwordState& ComponentSwordState::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const ComponentSwordState& ComponentSwordState::operator =(const std::string& _value)
    {
        if(_value == "appearance")
        {
            value = appearance;
            return *this;
        }
        if(_value == "prepare_to_fly")
        {
            value = prepare_to_fly;
            return *this;
        }
        if(_value == "fly")
        {
            value = fly;
            return *this;
        }
        return *this;
    }

    bool ComponentSwordState::operator ==(const ComponentSwordState& rhs) const
    {
        return value == rhs.value;
    }

    bool ComponentSwordState::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool ComponentSwordState::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool ComponentSwordState::operator ==(const std::string& rhs) const
    {
        return *this == ComponentSwordState(rhs);
    }

    bool operator ==(const std::string& lhs, const ComponentSwordState& rhs)
    {
        return ComponentSwordState(lhs) == rhs;
    }

    bool ComponentSwordState::operator <(const ComponentSwordState& rhs) const
    {
        return value < rhs.value;
    }

    ComponentSwordState::operator int() const
    {
        return value;
    }

    ComponentSwordState::operator std::string() const
    {
        if(value == appearance)
        {
            return "appearance";
        }
        if(value == prepare_to_fly)
        {
            return "prepare_to_fly";
        }
        if(value == fly)
        {
            return "fly";
        }
        return std::string();
    }

    std::string ComponentSwordState::str() const
    {
        if(value == appearance)
        {
            return "appearance";
        }
        if(value == prepare_to_fly)
        {
            return "prepare_to_fly";
        }
        if(value == fly)
        {
            return "fly";
        }
        return std::string();
    }

} //namespace mg
