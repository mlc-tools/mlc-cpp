//
//  Method.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include "Member.hpp"

struct Method : public Member {
    Member return_type;
    std::vector<Member> args;
    std::vector<std::string> body;
    
    bool is_external;
    bool is_abstract;
    bool is_template;
    bool is_virtual;
    bool is_friend;
    bool is_generate;
    bool translated;
    std::string specific_implementations;

    Method();
    Method(const Method &other) = default;
    Method(Method &&other) noexcept = default;
    Method& operator=(const Method &other)  = default;
    Method& operator=(Method &&other) noexcept  = default;
    
    virtual void set_modifier(const std::string& modifier) override;
};
