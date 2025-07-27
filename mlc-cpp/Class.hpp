//
//  Class.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include "Member.hpp"
#include "Method.hpp"
#include <set>


struct Class : public Member {
    std::string group;
    std::vector<std::string> includes;
    std::vector<Member> members;
    std::vector<Method> methods;
    std::vector<Method> constructors;

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

    Class() = default;
    Class(const Class &other) = default;
    Class(Class &&other) noexcept = default;
    Class& operator=(const Class &other) = default;
    Class& operator=(Class &&other) noexcept  = default;
    
    virtual void set_modifier(const std::string& modifier) override;
};
