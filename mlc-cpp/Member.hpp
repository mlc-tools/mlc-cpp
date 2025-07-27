//
//  Member.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include <string>
#include <vector>

enum class AccessSpecifier
{
    m_public,
    m_protected,
    m_private,
};

class Member {
public:
    std::string type;
    std::string name;
    std::string value;
    std::vector<Member> template_args;
    bool is_pointer;
    bool is_ref;
    bool is_runtime;
    bool is_static;
    bool is_const;
    bool is_key;
    bool is_link;
    std::string side;
    AccessSpecifier access;
//    self.denied_intrusive = False

    Member();
    virtual ~Member() = default;
    Member(const std::string& type_, const std::string& name_, const std::string& value_="");
    Member(const Member &other) = default;
    Member(Member &&other) noexcept = default;
    Member& operator=(const Member &other) = default;
    Member& operator=(Member &&other) noexcept = default;
    
    virtual void set_modifier(const std::string& modifier);
};
