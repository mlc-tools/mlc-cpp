//
//  Method.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Method.hpp"
#include "Modifier.h"

Method::Method()
: is_external(false)
, is_abstract(false)
, is_template(false)
, is_virtual(false)
, is_friend(false)
, is_generate(false)
, translated(false){
}

void Method::set_modifier(const std::string& modifier)
{
    if(modifier == Modifier::m_external) this->is_external = true;
    else if(modifier == Modifier::m_abstract) this->is_abstract = true;
    else if(modifier == Modifier::m_virtual) this->is_virtual = true;
    else Member::set_modifier(modifier);
}
