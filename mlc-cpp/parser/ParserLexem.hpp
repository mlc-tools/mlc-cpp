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

class ParserLexem {
public:
    explicit ParserLexem(const std::string &src);
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
    std::string skip_body();
    std::string read_until(const std::set<std::string>& symbols);
    std::vector<Object> read_templates();
public:
    Object parse_member(bool with_name, bool is_enum);
    void parse_constructor(Class &cls);
    void parse_method(Class &cls);
    std::vector<Object> parse_method_args();
};
Object parse_object(const std::string& str);
