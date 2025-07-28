//
//  Class.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include "Object.hpp"
#include "Function.hpp"
#include <set>
#include <memory>

class Model;

struct Class : public Object {
    std::string group;
    std::vector<std::string> includes;
    std::vector<Object> members;
    std::vector<Function> functions;
    std::vector<Function> constructors;

    std::string parent_class_name;
    std::weak_ptr<Class> parent;
    std::vector<std::weak_ptr<Class>> subclasses;
    std::vector<std::shared_ptr<Class>> inner_classes;
    std::set<std::string> user_includes;
    bool is_abstract = false;
    bool is_serialized = false;
    bool is_visitor = false;
    bool is_storage = false;
    bool is_numeric = false;
    bool is_test = false;
    bool is_inline = false;
    bool is_virtual = false;
    bool is_enum = false;
    bool generate_set_function = false;
    bool auto_generated = true;
    bool _linked = false;
    bool prefer_use_forward_declarations = false;
    
    std::string inner_body;

    Class() = default;
    Class(const Class &other) = default;
    Class(Class &&other) noexcept = default;
    Class& operator=(const Class &other) = default;
    Class& operator=(Class &&other) noexcept  = default;
    
    virtual void set_modifier(const std::string_view& modifier) override;
    
    void onLinked(Model& model);
};
