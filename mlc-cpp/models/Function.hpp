//
//  Method.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include "Object.hpp"

struct Function : public Object {
    Object return_type;
    std::string body;

    bool is_external;
    bool is_abstract;
    bool is_template;
    bool is_virtual;
    bool is_friend;
    bool is_generate;
    bool translated;
    std::string specific_implementations;

    Function();
    Function(const Function &other) = default;
    Function(Function &&other) noexcept = default;
    Function &operator=(const Function &other) = default;
    Function &operator=(Function &&other) noexcept = default;
    virtual bool is_equal_declaration(const Function &other) const;

    virtual void set_modifier(const std::string_view &modifier) override;
};
