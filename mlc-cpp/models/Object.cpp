//
//  Member.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Object.hpp"
#include "Modifier.h"
#include <cassert>

Object::Object()
: is_pointer(false)
, is_ref(false)
, is_runtime(false)
, is_static(false)
, is_const(false)
, is_key(false)
, is_link(false)
, side("both")
, access(AccessSpecifier::m_public) {
}

Object::Object(const std::string& type_, const std::string& name_, const std::string& value_)
: type(type_)
, name(name_)
, value(value_)
, is_pointer(false)
, is_ref(false)
, is_runtime(false)
, is_static(false)
, is_const(false)
, is_key(false)
, is_link(false)
, side("both")
, access(AccessSpecifier::m_public) {
}


void Object::set_modifier(const std::string_view& modifier)
{
    if(modifier == Modifier::m_private) access = AccessSpecifier::m_private;
    else if(modifier == Modifier::m_protected) access = AccessSpecifier::m_protected;
    else if(modifier == Modifier::m_public) access = AccessSpecifier::m_public;
    else if(modifier == Modifier::m_pointer) this->is_pointer = true;
    else if(modifier == Modifier::m_ref) this->is_ref = true;
    else if(modifier == Modifier::m_ref) this->is_ref = true;
    else if(modifier == Modifier::m_runtime) this->is_runtime = true;
    else if(modifier == Modifier::m_static) this->is_static = true;
    else if(modifier == Modifier::m_const) this->is_const = true;
    else if(modifier == Modifier::m_key) this->is_key = true;
    else if(modifier == Modifier::m_link) this->is_link = true;
    else if(modifier == Modifier::m_client) this->side = modifier;
    else if(modifier == Modifier::m_server) this->side = modifier;
    else if(modifier == Modifier::m_test) this->is_test = true;
    
    else if(modifier == Modifier::l_cpp) this->lang_specific.insert(Modifier::l_cpp);
    else if(modifier == Modifier::l_py) this->lang_specific.insert(Modifier::l_py);
    else if(modifier == Modifier::l_php) this->lang_specific.insert(Modifier::l_php);
    else if(modifier == Modifier::l_js) this->lang_specific.insert(Modifier::l_js);
    
    else assert(0);
}
