//
//  Member.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Object.hpp"
#include "Modifier.h"
#include <cassert>
#include <iostream>


namespace Objects
{
Object VOID("void", "");
Object BOOL("bool", "");
Object INT("int", "");
Object UINT("unsigned", "");
Object INT_64("int64_t", "");
Object UINT_64("uint64_t", "");
Object FLOAT("float", "");
Object DOUBLE("double", "");
Object STRING("string", "");
}

std::string AccessSpecifierToString(AccessSpecifier value)
{
    if(value == AccessSpecifier::m_public) return "public";
    if(value == AccessSpecifier::m_protected) return "protected";
    if(value == AccessSpecifier::m_private) return "private";
    assert(0);
    return "";
}

Object::Object()
: is_pointer(false)
, is_ref(false)
, is_runtime(false)
, is_static(false)
, is_const(false)
, is_key(false)
, is_link(false)
, side(Side::both)
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
, side(Side::both)
, access(AccessSpecifier::m_public) {
}

bool Object::is_equal(const Object &other) const
{
    bool result = true;
    result = result && type == other.type;
    result = result && name == other.name;
    result = result && template_args.size() == other.template_args.size();
    if(result)
    {
        for(size_t i=0; i<template_args.size(); ++i)
        {
            result = result && template_args.at(i).is_equal(other.template_args.at(i));
        }
    }
    if(result)
    {
        for(size_t i=0; i<callable_args.size(); ++i)
        {
            result = result && callable_args.at(i).is_equal(other.callable_args.at(i));
        }
    }
    return result;
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
    else if(modifier == Modifier::m_client)
        this->side = Side::client;
    else if(modifier == Modifier::m_server)
        this->side = Side::server;
    else if(modifier == Modifier::m_test) this->is_test = true;
    
    else if(modifier == Modifier::l_cpp) this->lang_specific.insert(Modifier::l_cpp);
    else if(modifier == Modifier::l_py) this->lang_specific.insert(Modifier::l_py);
    else if(modifier == Modifier::l_php) this->lang_specific.insert(Modifier::l_php);
    else if(modifier == Modifier::l_js) this->lang_specific.insert(Modifier::l_js);
    
    else assert(0);
}
void Object::set_default_initial_value()
{
    if(type == "unsigned")
        type = Objects::UINT.type;
    
    if(!value.empty())
        return;
    if(type == Objects::INT.type ||
       type == Objects::UINT.type ||
       type == Objects::INT_64.type ||
       type == Objects::UINT_64.type)
    {
        value = "0";
    }
    else if(type == Objects::FLOAT.type) value = "0.0f";
    else if(type == Objects::DOUBLE.type) value = "0.0";
    else if(type == Objects::BOOL.type) value = "false";
    else if(type == Objects::STRING.type) value = "\"\"";
    else if(is_pointer) value = "nullptr";
}
