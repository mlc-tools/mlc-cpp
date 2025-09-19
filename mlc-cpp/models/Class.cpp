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

void Class::set_modifier(const std::string_view& modifier)
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

bool Class::has_member(const std::string& name) const
{
    for(auto& m : this->members){
        if(m.name == name){
            return true;
        }
    }
    return false;
}

bool Class::has_method(const std::string& name) const
{
    for(auto& m : this->functions){
        if(m.name == name){
            return true;
        }
    }
    return false;
}

Function* Class::get_method(const std::string& name)
{
    for(auto& method : this->functions){
        if(method.name == name){
            return &method;
        }
    }
    return nullptr;
}

void Class::onLinked(Model& model)
{
    
}

bool Class::has_virtual() const
{
    bool result = false;
    result = result || this->is_virtual;
    //TODO: check this code
//    result = result or this->superclasses;
//    result = result or this->subclasses;
    result = result || this->has_abstract_method();
    if(!result){
        for(auto cls : this->subclasses){
            result = result || cls.lock()->has_virtual();
            if(result)
                return true;
        }
    }
    return result;
}

bool Class::has_abstract_method() const
{
    for(auto& f : functions)
        if(f.is_abstract)
            return true;
    return false;
}
