//
//  Class.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Class.hpp"
#include "Modifier.h"

/*
Class* parent = nullptr;
std::vector<Class*> subclasses;
std::vector<Class*> inner_classes;
std::set<std::string> user_includes;
bool is_abstract = false;
bool is_serialized = false;
bool is_visitor = false;
bool is_storage = false;
bool is_numeric = false;
bool is_test = false;
bool is_inline = false;
bool is_virtual = false;
bool generate_set_function = false;
bool auto_generated = true;
bool _linked = false;
bool prefer_use_forward_declarations = false;
*/

void Class::set_modifier(const std::string& modifier)
{
    if(modifier == Modifier::m_abstract) this->is_abstract = true;
    else if(modifier == Modifier::m_visitor) this->is_visitor = true;
    else if(modifier == Modifier::m_storage) this->is_storage = true;
    else if(modifier == Modifier::m_numeric) this->is_numeric = true;
    else if(modifier == Modifier::m_test) this->is_test = true;
    else if(modifier == Modifier::m_inline) this->is_inline = true;
    else if(modifier == Modifier::m_virtual) this->is_virtual = true;
    else if(modifier == Modifier::m_prefer_use_forward_declarations) this->prefer_use_forward_declarations = true;
    else Object::set_modifier(modifier);
}


void Class::onLinked(Model& model)
{
    
}
