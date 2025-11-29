//
//  Class.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include "Function.hpp"
#include "Object.hpp"
#include <memory>
#include <set>

class Model;

struct Class : public Object {
    // Исходный .mlc файл, из которого распарсен этот класс
    std::string source_path;
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
    bool has_bindings = false;

    std::string inner_body;

    Class() = default;
    Class(const Class &other) = default;
    Class(Class &&other) noexcept = default;
    Class &operator=(const Class &other) = default;
    Class &operator=(Class &&other) noexcept = default;

    virtual void set_modifier(const std::string_view &modifier) override;
    void generate_constructor();

    bool has_member(const std::string &name) const;
    bool has_method(const std::string &name) const;
    bool has_method(const Function &func) const;
    Function *get_method(const std::string &name);
    bool has_virtual() const;
    bool has_virtual_method() const;
    bool has_abstract_method() const;
    bool has_function_in_subclasses(const Function &func, bool depth = 0);
    bool has_function_in_parentclass(const Function &func, bool depth = 0);

    void onLinked(Model &model);
};
