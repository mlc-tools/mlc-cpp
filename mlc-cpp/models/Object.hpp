//
//  Member.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include <string>
#include <vector>
#include <set>

enum class AccessSpecifier
{
    m_public,
    m_protected,
    m_private,
};

class Object {
public:
    std::string type;
    std::string name;
    std::string value;
    std::set<std::string> lang_specific;
    std::vector<Object> template_args;
    std::vector<Object> callable_args;
    bool is_pointer = false;
    bool is_ref = false;
    bool is_runtime = false;
    bool is_static = false;
    bool is_const = false;
    bool is_key = false;
    bool is_link = false;
    bool is_test = false;
    std::string side;
    AccessSpecifier access;
//    self.denied_intrusive = False

    Object();
    virtual ~Object() = default;
    Object(const std::string& type_, const std::string& name_, const std::string& value_="");
    Object(const Object &other) = default;
    Object(Object &&other) noexcept = default;
    Object& operator=(const Object &other) = default;
    Object& operator=(Object &&other) noexcept = default;
    
    virtual void set_modifier(const std::string_view& modifier);
};
