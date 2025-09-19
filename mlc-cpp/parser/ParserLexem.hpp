//
//  ParserLexem.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <set>
#include "Lexer.hpp"

class Class;
class Object;
class Function;
class Model;

class ParserLexem {
public:
    explicit ParserLexem(const std::string &src, Model& model);
    void read_class_name_and_group(std::shared_ptr<Class>& cls);
    void read_modifier(Object& object);
    void read_parent_class(std::shared_ptr<Class>& cls);
    std::vector<std::shared_ptr<Class>> parse_dict(bool one=false);
    void parse(const std::shared_ptr<Class>& cls);
private:
    Lexer lexer;
    Token cur;
    
    void advance();
    
    std::vector<std::string> read_body();
    std::string_view skip_body();
    std::vector<Object> read_templates();
public:
    Model& _model;
    Object parse_member(bool with_name, bool is_enum);
    void parse_constructor(Class &cls);
    Function parse_method();
    std::vector<Object> read_callable_args();
};
