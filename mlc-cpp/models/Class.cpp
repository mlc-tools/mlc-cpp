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

void Class::set_modifier(const std::string_view &modifier) {
    if (modifier == Modifier::m_abstract)
        this->is_abstract = true;
    else if (modifier == Modifier::m_visitor)
        this->is_visitor = true;
    else if (modifier == Modifier::m_storage)
        this->is_storage = true;
    else if (modifier == Modifier::m_numeric)
        this->is_numeric = true;
    else if (modifier == Modifier::m_test)
        this->is_test = true;
    else if (modifier == Modifier::m_inline)
        this->is_inline = true;
    else if (modifier == Modifier::m_virtual)
        this->is_virtual = true;
    else if (modifier == Modifier::m_discard_virtual)
        this->discard_virtual = true;
    else if (modifier == Modifier::m_prefer_use_forward_declarations)
        this->prefer_use_forward_declarations = true;
    else if (modifier == Modifier::m_binding)
        this->has_bindings = true;
    else
        Object::set_modifier(modifier);
}

void Class::generate_constructor() {
    for (auto &constructor : this->constructors) {
        if (constructor.is_generate) {
            for (auto &member : this->members) {
                Object arg;
                arg.type = member.type;
                arg.name = member.name;
                arg.set_default_initial_value();
                constructor.callable_args.push_back(std::move(arg));
                constructor.body +=
                    "this->" + member.name + " = " + member.name + ";\n";
            }
        }
    }
}

bool Class::has_member(const std::string &name) const {
    for (auto &m : this->members) {
        if (m.name == name) {
            return true;
        }
    }
    return false;
}

bool Class::has_method(const std::string &name) const {
    for (auto &m : this->functions) {
        if (m.name == name) {
            return true;
        }
    }
    return false;
}

bool Class::has_method(const Function &func) const {
    auto iter = std::find_if( functions.begin(), functions.end(), [func](const Function &f) {
        return func.is_equal_declaration(f);
    });
    return iter != functions.end();
}

Function *Class::get_copy_constructor(){
    for(auto& func : functions){
        if(func.name == name &&
           func.callable_args.size() == 1 &&
           func.callable_args.at(0).type == name &&
           func.callable_args.at(0).is_ref &&
           func.callable_args.at(0).is_const &&
           func.callable_args.at(0).template_args.empty() &&
           func.is_static == false
           ){
            return &func;
        }
    }
    return nullptr;
}

Function* Class::get_move_constructor(){
    for(auto& func : functions){
        if(func.name == name &&
           func.callable_args.size() == 1 &&
           func.callable_args.at(0).type == name &&
           func.callable_args.at(0).is_ref &&
           func.callable_args.at(0).is_rvalue &&
           func.callable_args.at(0).template_args.empty() &&
           func.is_static == false
           ){
            return &func;
        }
    }
    return nullptr;
}
Function* Class::get_copy_operator(){
    for(auto& func : functions){
        if(func.name == "operator =" &&
           func.callable_args.size() == 1 &&
           func.callable_args.at(0).type == name &&
           func.callable_args.at(0).is_ref &&
           func.callable_args.at(0).is_const &&
           func.callable_args.at(0).template_args.empty() &&
           func.is_static == false
           ){
            return &func;
        }
    }
    return nullptr;
}
Function* Class::get_move_operator(){
    for(auto& func : functions){
        if(func.name == "operator =" &&
           func.callable_args.size() == 1 &&
           func.callable_args.at(0).type == name &&
           func.callable_args.at(0).is_ref &&
           func.callable_args.at(0).is_rvalue &&
           func.callable_args.at(0).template_args.empty() &&
           func.is_static == false
           ){
            return &func;
        }
    }
    return nullptr;
}

Function *Class::get_method(const std::string &name) {
    for (auto &method : this->functions) {
        if (method.name == name) {
            return &method;
        }
    }
    return nullptr;
}

void Class::onLinked(Model &model) {
    if (_linked)
        return;
    
    if (!parent.expired()){
        this->discard_virtual = parent.lock()->is_discard_virtual();
        this->discard_copy_ctr = parent.lock()->is_discard_copy_ctr();
        this->discard_copy = parent.lock()->is_discard_copy();
    }

    if(!discard_virtual){
        for (auto &func : functions) {
            if (func.name.find("operator") == 0) {
                func.is_virtual = false;
                continue;
            }
            func.is_virtual = is_virtual || func.is_virtual || func.is_abstract ||
            has_function_in_subclasses(func) ||
            has_function_in_parentclass(func);
        }
        
        if (!is_abstract) {
            for (auto &method : functions) {
                if (method.is_abstract) {
                    is_abstract = true;
                    method.is_virtual = true;
                    break;
                }
            }
        }
    }
    if (!parent.expired()){
        parent.lock()->onLinked(model);
    }
    
    _linked = true;
}

bool Class::has_virtual() const {
    bool result = false;
    if(this->discard_virtual)
        return false;
    result = result || this->is_virtual;
    result = result || !this->parent.expired();
    result = result || !this->subclasses.empty();
    result = result || this->has_abstract_method();
    result = result || this->has_virtual_method();
    if (!result) {
        for (auto cls : this->subclasses) {
            result = result || cls.lock()->has_virtual();
            if (result)
                return true;
        }
    }
    return result;
}

bool Class::has_virtual_method() const{
    for (auto &f : functions)
        if (f.is_virtual)
            return true;
    return false;
}

bool Class::has_abstract_method() const {
    for (auto &f : functions)
        if (f.is_abstract)
            return true;
    return false;
}

bool Class::has_function_in_subclasses(const Function &func, bool depth) {
    for (auto wclass : subclasses) {
        auto cls = wclass.expired() ? nullptr : wclass.lock();
        if (cls->has_function_in_subclasses(func, depth + 1))
            return true;
    }
    if (depth > 0) {
        return has_method(func);
    }
    return false;
}

bool Class::has_function_in_parentclass(const Function &func, bool depth) {
    if (!parent.expired() &&
        parent.lock()->has_function_in_parentclass(func, depth + 1)) {
        return true;
    }
    if (depth > 0) {
        return has_method(func);
    }
    return false;
}

bool Class::is_discard_virtual() const{
    return this->discard_virtual || (!parent.expired() && parent.lock()->is_discard_virtual());
}

bool Class::is_discard_copy_ctr() const{
    if(this->discard_copy_ctr || (!parent.expired() && parent.lock()->is_discard_copy_ctr()))
        return true;
    for(auto& obj : members){
        if(obj.is_discard_copy_ctr())
            return true;
    }
    return false;
}
bool Class::is_discard_copy() const{
    if(this->discard_copy || (!parent.expired() && parent.lock()->is_discard_copy()))
        return true;
    for(auto& obj : members){
        if(obj.is_discard_copy())
            return true;
    }
    return false;
}
bool Class::is_discard_move() const{
    if(this->discard_move || (!parent.expired() && parent.lock()->is_discard_move()))
        return true;
    for(auto& obj : members){
        if(obj.is_discard_move())
            return true;
    }
    return false;
}
