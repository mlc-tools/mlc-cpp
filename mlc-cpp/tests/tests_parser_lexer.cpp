//
//  tests.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#include "../core/Parser.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Model.hpp"
#include "../parser/ParserLexem.hpp"
#include "tests.hpp"
#include <cassert>
#include <iostream>
#include <map>


template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
void serialize(const Map& values){
    
}

namespace tests {
extern void run_regex_tests();
extern void run_models_tests();

void test_lexer_basic_tokens() {
    std::map<int, int> m;
    std::unordered_map<int, int> m2;
    serialize(m);
    serialize(m2);
    
    std::string code = "@include class MyClass:public { }";
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
    Model model;
    ParserLexem parser(code, model);

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
            @include Other
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
    assert(cls->user_includes.size() == 1);
    ;
    assert(cls->user_includes.count("Other") == 1);
}

void test_parser_class_with_inner() {
    Model m;
    Parser p(m);
    std::string code = R"(
        class group_name/MyClass {
            class InnerClass{}
            InnerClass inner_class;
        }
    )";
    p.parseText(code);

    assert(m.classes.size() == 2);
    auto cls = m.classes[0];
    assert(cls->name == "MyClass");
    assert(cls->group == "group_name");
    assert(cls->members.size() == 1);
    assert(cls->members[0].type == "MyClassInnerClass");
    assert(cls->members[0].name == "inner_class");

    cls = m.classes[1];
    assert(cls->name == "MyClassInnerClass");
    assert(cls->group == "group_name");
}

void test_parse_members() {
    auto check_object = [](const Object &obj, const std::string &type,
                           const std::string &name,
                           const std::string &value = "") {
        assert(obj.type == type);
        assert(obj.name == name);
        assert(value.empty() || obj.value == value);
        return obj;
    };
    auto check_templates = [](const Object &obj,
                              const std::vector<std::string> &args) {
        assert(obj.template_args.size() == args.size());
        for (size_t i = 0; i < args.size(); ++i)
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
    check_object(parse_object("set<int>", false), "set", "");
    check_object(parse_object("hash_set<int>", false), "hash_set", "");
    check_object(parse_object("hash_map<int>", false), "hash_map", "");
    check_templates(check_object(parse_object("map<int, float> m", true), "map", "m"), {"int", "float"});

    check_templates( check_object(parse_object("map<int, list<float>> m", true), "map", "m"), {"int", "list"});

    check_object(parse_object("float t=0", true), "float", "t", "0");
    check_object(parse_object("float t=0.1f", true), "float", "t", "0.1f");
    check_object(parse_object("float t=-0.1f", true), "float", "t", "-0.1f");
    check_object(parse_object("float FLOAT_SMALL = 2e-37f", true), "float",
                 "FLOAT_SMALL", "2e-37f");
    check_object(parse_object("bool:protected _breaked = false", true), "bool",
                 "_breaked", "false");

    auto obj = parse_object("Vector:const:static:private ZERO", true);
    check_object(obj, "Vector", "ZERO");
    assert(obj.is_static);
    assert(obj.is_const);
    assert(obj.access == AccessSpecifier::m_private);

    obj = parse_object("ModelEcsBase* model", true);
    check_object(obj, "ModelEcsBase", "model");
    assert(obj.is_pointer);

    obj = parse_object(
        "Observable<void(DataUnit:link, DataSkin:link)>:runtime:client "
        "event_current_skin_changed",
        true);
    check_object(obj, "Observable", "event_current_skin_changed");
    assert(obj.template_args.size() == 1 &&
           obj.template_args.at(0).callable_args.size() == 2);
    assert(obj.template_args.at(0).callable_args.at(0).type == "DataUnit" &&
           obj.template_args.at(0).callable_args.at(0).is_link);
    assert(obj.template_args.at(0).callable_args.at(1).type == "DataSkin" &&
           obj.template_args.at(0).callable_args.at(1).is_link);

    obj =
        parse_object("Observable<void(Vector:ref:const pos, float lifet_time, "
                     "float radius, string name)>:runtime event_create_marker",
                     true);
    check_object(obj, "Observable", "event_create_marker");
    assert(obj.template_args.size() == 1 &&
           obj.template_args.at(0).callable_args.size() == 4);
    assert(obj.template_args.at(0).callable_args.at(0).type == "Vector" &&
           obj.template_args.at(0).callable_args.at(0).is_ref);
    assert(obj.template_args.at(0).callable_args.at(1).type == "float" &&
           obj.template_args.at(0).callable_args.at(1).name == "lifet_time");
    assert(obj.template_args.at(0).callable_args.at(2).type == "float" &&
           obj.template_args.at(0).callable_args.at(2).name == "radius");
    assert(obj.template_args.at(0).callable_args.at(3).type == "string" &&
           obj.template_args.at(0).callable_args.at(3).name == "name");

    obj = parse_object("FooEnum enumFoo = FooEnum::foo", true);
    check_object(obj, "FooEnum", "enumFoo", "FooEnum::foo");

    obj = parse_object("FooEnum:client enumFoo = FooEnum::foo", true);
    assert(obj.side == Side::client);
    obj = parse_object("FooEnum:server enumFoo = FooEnum::foo", true);
    assert(obj.side == Side::server);
}

void test_parse_functions() {
    auto check_func = [](const Function &func, const std::string &return_type,
                         const std::string &name) {
        assert(func.return_type.type == return_type);
        assert(func.name == name);
        return func;
    };
    auto check_args = [](const Function &func,
                         const std::map<std::string, std::string> &args) {
        assert(func.callable_args.size() == args.size());
        for (size_t i = 0; i < args.size(); ++i) {
            assert(args.count(func.callable_args[i].type) > 0);
            assert(func.callable_args[i].name ==
                   args.at(func.callable_args[i].type));
        }
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

    check_args(parse_function("fn void foo(int a, float b){}"),
               {std::make_pair("int", "a"), std::make_pair("float", "b")});

    auto func =
        parse_function("fn Vector:ref operator+=(Vector:ref:const v):cpp");
    check_func(func, "Vector", "operator+=");
    assert(func.callable_args.size() == 1 &&
           func.callable_args[0].type == "Vector" &&
           func.callable_args[0].name == "v" && func.callable_args[0].is_ref &&
           func.callable_args[0].is_const);
    assert(func.return_type.is_ref);

    func = parse_function("fn void copy_hero(int other_id, int self_id) { auto "
                          "self_hp = this->get<Health>(self_id); auto other_hp "
                          "= this->model_inner->get<Health>(other_id); "
                          "self_hp->value = other_hp->value; }");
    check_func(func, "void", "copy_hero");

    func =
        parse_function("fn<T> void assertEqual(T left, T right, string message="
                       ")");
    check_func(func, "void", "assertEqual");
    assert(func.template_args.size() == 1);
    assert(func.template_args.at(0).type == "T");
    assert(func.callable_args.size() == 3);
    assert(func.callable_args.at(0).type == "T" &&
           func.callable_args.at(0).name == "left");
    assert(func.callable_args.at(1).type == "T" &&
           func.callable_args.at(1).name == "right");
    assert(func.callable_args.at(2).type == "string" &&
           func.callable_args.at(2).name == "message");

    func = parse_function("fn void add_result(bool result, string message)");
    assert(func.callable_args.size() == 2);
    assert(func.callable_args.at(0).value == "");
    assert(func.callable_args.at(1).value == "");
}

void run() {
    test_lexer_basic_tokens();
    test_parser_simple_class();
    test_parser_remove_comments();
    test_parser_class_with_member();
    test_parser_class_with_inner();
    test_parse_members();
    test_parse_functions();
    run_regex_tests();
    run_models_tests();

    std::cout << "All tests passed!\n";
}

} // namespace tests
