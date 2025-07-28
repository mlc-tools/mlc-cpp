//
//  tests.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#include <cassert>
#include <iostream>
#include "tests.hpp"
#include "../parser/ParserLexem.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Class.hpp"
#include "../models/Model.hpp"
#include "../core/Parser.hpp"

namespace tests
{
void test_lexer_basic_tokens() {
    std::string code = "@include class MyClass :public { }";
    Lexer lexer(code);

    Token t = lexer.next();
    assert(t.type == TokenType::Include && t.value == "@include");

    t = lexer.next();
    assert(t.type == TokenType::Keyword && t.value == "class");

    t = lexer.next();
    assert(t.type == TokenType::Identifier && t.value == "MyClass");

    t = lexer.next();
    assert(t.type == TokenType::Symbol && t.value == ":");

    t = lexer.next();
    assert(t.type == TokenType::Identifier && t.value == "public");

    t = lexer.next();
    assert(t.type == TokenType::Symbol && t.value == "{");

    t = lexer.next();
    assert(t.type == TokenType::Symbol && t.value == "}");

    t = lexer.next();
    assert(t.type == TokenType::Eof);
}

void test_parser_simple_class() {
    std::string code = "class TestClass { int x }";
    ParserLexem parser(code);

    auto classes = parser.parse_dict();
    assert(classes.size() == 1);
    assert(classes[0]->name == "TestClass");
    assert(!classes[0]->is_enum);
    assert(!classes[0]->is_abstract);
}

void test_parser_remove_comments() {
    Model m;
    Parser p(m);
    std::string input = R"(
int a; // comment
/* block */int b;
)";
    std::string output = p.removeComments(input);

    assert(output.find("//") == std::string::npos);
    assert(output.find("/*") == std::string::npos);
    assert(output.find("int a;") != std::string::npos);
    assert(output.find("int b;") != std::string::npos);
}

void test_parser_class_with_member() {
    Model m;
    Parser p(m);
    std::string code = R"(
        class MyClass {
            int value
            fn void method() { }
        }
    )";
    p.parseText(code);

    assert(m.classes.size() == 1);
    auto cls = m.classes[0];
    assert(cls->name == "MyClass");
    assert(cls->members.size() == 1);
    assert(cls->members[0].type == "int");
    assert(cls->members[0].name == "value");
    assert(cls->functions.size() == 1);
    assert(cls->functions[0].name == "method");
}

void test_parse_members() {
    auto check_object = [](const Object& obj, const std::string& type, const std::string& name, const std::string& value=""){
        assert(obj.type == type);
        assert(obj.name == name);
        assert(obj.value == value);
        return obj;
    };
    auto check_templates = [](const Object& obj, const std::vector<std::string>& args){
        assert(obj.template_args.size() == args.size());
        for(size_t i=0; i<args.size();++i)
            assert(obj.template_args[i].type == args[i]);
        return obj;
    };
    check_object(parse_object("int a", true), "int", "a");
    check_object(parse_object("int a=0", true), "int", "a", "0");
    check_object(parse_object("int a = 0", true), "int", "a", "0");
    check_object(parse_object("int", false), "int", "");
    check_object(parse_object("float", false), "float", "");
    check_object(parse_object("bool", false), "bool", "");
    check_object(parse_object("string", false), "string", "");
    check_object(parse_object("list<int>", false), "list", "");
    check_object(parse_object("list<float>", false), "list", "");
    check_object(parse_object("list<float> l", true), "list", "l");
    check_object(parse_object("map<int, int>", false), "map", "");
    check_templates(check_object(parse_object("map<int, float> m", true), "map", "m"), {"int", "float"});;
    check_templates(check_object(parse_object("map<int, list<float>> m", true), "map", "m"), {"int", "list"});;
    check_object(parse_object("float t=0", true), "float", "t", "0");
    check_object(parse_object("float t=0.1f", true), "float", "t", "0.1f");
    check_object(parse_object("float t=-0.1f", true), "float", "t", "-0.1f");
    check_object(parse_object("float FLOAT_SMALL = 2e-37f", true), "float", "FLOAT_SMALL", "2e-37f");
    check_object(parse_object("bool:protected _breaked = false", true), "bool", "_breaked", "false");
    
    auto obj = parse_object("Vector:const:static:private ZERO", true);
    check_object(obj, "Vector", "ZERO");
    assert(obj.is_static);
    assert(obj.is_const);
    assert(obj.access == AccessSpecifier::m_private);
    
    obj = parse_object("ModelEcsBase* model", true);
    check_object(obj, "ModelEcsBase", "model");
    assert(obj.is_pointer);
    
}


void test_parse_functions() {
    auto check_func = [](const Function& func, const std::string& return_type, const std::string& name){
        assert(func.return_type.type == return_type);
        assert(func.name == name);
        return func;
    };
    check_func(parse_function("fn void foo(){}"), "void", "foo");
    check_func(parse_function("fn void operator+=(){}"), "void", "operator+=");
    check_func(parse_function("fn void operator-=(){}"), "void", "operator-=");
    check_func(parse_function("fn void operator*=(){}"), "void", "operator*=");
    check_func(parse_function("fn void operator/=(){}"), "void", "operator/=");
    check_func(parse_function("fn void operator+(){}"), "void", "operator+");
    check_func(parse_function("fn void operator-(){}"), "void", "operator-");
    check_func(parse_function("fn void operator*(){}"), "void", "operator*");
    check_func(parse_function("fn void operator/(){}"), "void", "operator/");
    
    
    auto func = parse_function("fn Vector:ref operator+=(Vector:ref:const v):cpp");
    check_func(func, "Vector", "operator+=");
    assert(func.args.size() == 1 && func.args[0].type == "Vector" && func.args[0].name == "v" && func.args[0].is_ref && func.args[0].is_const);
    assert(func.return_type.is_ref);
    
}

void run() {
    test_lexer_basic_tokens();
    test_parser_simple_class();
    test_parser_remove_comments();
    test_parser_class_with_member();
    test_parse_members();
    test_parse_functions();

    std::cout << "All tests passed!\n";
}

}
