//
//  Member.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include <set>
#include <string>
#include <vector>

enum class AccessSpecifier {
    m_public,
    m_protected,
    m_private,
};

enum class Side : int {
    client = 1,
    server = 2,
    both = 3,
};

std::string AccessSpecifierToString(AccessSpecifier value);

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
    bool is_binding = false;
    Side side;
    AccessSpecifier access;
    bool denied_intrusive = false;

    Object();
    virtual ~Object() = default;
    Object(const std::string &type_, const std::string &name_,
           const std::string &value_ = "");
    Object(const Object &other) = default;
    Object(Object &&other) noexcept = default;
    Object &operator=(const Object &other) = default;
    Object &operator=(Object &&other) noexcept = default;
    virtual bool is_equal(const Object &other) const;

    virtual void set_modifier(const std::string_view &modifier);
    void set_default_initial_value();

};

namespace Objects {
extern Object VOID;
extern Object INT;
extern Object UINT;
extern Object INT_64;
extern Object UINT_64;
extern Object BOOL;
extern Object FLOAT;
extern Object DOUBLE;
extern Object STRING;

} // namespace Objects
