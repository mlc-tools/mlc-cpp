//
//  ParserLexem.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#include <cassert>
#include <iostream>
#include "ParserLexem.hpp"
#include "Class.hpp"
#include "Function.hpp"

template <class T>
bool in(const T& string, const std::set<T>& variants) {
    return variants.count(string) > 0;
}

ParserLexem::ParserLexem(const std::string &src) : lexer(src) {
    advance();
}

void ParserLexem::read_class_name_and_group(std::shared_ptr<Class>& cls)
{
    if (cur.type == TokenType::Identifier) {
        cls->name = cur.value;
        advance();
        if(cur.type == TokenType::Symbol && cur.value == "/") {
            cls->group = std::move(cls->name);
            advance();
            assert(cur.type == TokenType::Identifier);
            cls->name = cur.value;
            advance();
        }
    }
}
void ParserLexem::read_modifier(Object& object)
{
    while(cur.type == TokenType::Symbol && cur.value == ":") {
        advance();
        if (cur.type == TokenType::Identifier) {
            object.set_modifier(cur.value);
            advance();
        }
        else {
            assert(false);
        }
    }
}
void ParserLexem::read_parent_class(std::shared_ptr<Class>& cls){
    if(cur.type == TokenType::Symbol && cur.value == "<") {
        advance();
        if (cur.type == TokenType::Identifier) {
            cls->parent_class_name = cur.value;
            advance();
            if(cur.type == TokenType::Symbol && cur.value == ">")
                advance();
        }
    }
}

std::vector<std::shared_ptr<Class>> ParserLexem::parse_dict(bool one) {
    std::vector<std::shared_ptr<Class>> result;
    // ищем class
    while (cur.type != TokenType::Eof) {
        if (cur.type == TokenType::Keyword && (cur.value == "class" || cur.value == "enum" || cur.value == "interface")) {
            result.push_back(std::make_shared<Class>());
            auto cls = result.back();
            cls->is_enum = cur.value == "enum";
            cls->is_abstract = cur.value == "interface";
            advance();

            read_class_name_and_group(cls);
            read_parent_class(cls);
            read_modifier(*cls);
            cls->inner_body = skip_body();
            advance();
            if(one)
                break;
        } else {
            std::cout << "Skip on parse dict: " << cur.value << std::endl;
            advance();
        }
    }
    
    return result;
}

void ParserLexem::parse(const std::shared_ptr<Class>& cls) {
    int depth = 0;
    // ищем class
    while (cur.type != TokenType::Eof) {
        if (cur.type == TokenType::Keyword && (cur.value == "class" || cur.value == "enum" || cur.value == "interface")) {
            auto inner = parse_dict(true);
            cls->inner_classes.insert(cls->inner_classes.end(), inner.begin(), inner.end());
        }
        else if (cur.type == TokenType::Include) {
            advance();
            if (cur.type == TokenType::Identifier) {
                cls->includes.push_back(std::string(cur.value));
                advance();
            }
        } else if (cur.type == TokenType::Keyword && cur.value == "constructor") {
            parse_constructor(*cls); // пока игнорируем тело конструктора
        } else if (cur.type == TokenType::Keyword && cur.value == "fn") {
            cls->functions.push_back(parse_method());
        } else if (cur.type == TokenType::Identifier) {
            cls->members.push_back(parse_member(true, cls->is_enum));
        } else if (cur.type == TokenType::Symbol && cur.value == "{") {
            ++depth;
            advance();
        } else if (cur.type == TokenType::Symbol && cur.value == "}") {
            --depth;
            advance();
            assert(depth >= 0);
        } else if(cur.type == TokenType::Symbol && cur.value == ";") {
            //log unused ;
            advance();
        } else {
            std::cout << "Skip: " << cur.value << std::endl;
            advance();
        }
    }
}

void ParserLexem::advance() { cur = lexer.next(); }

std::vector<std::string> ParserLexem::read_body() {
    std::vector<std::string> block;
    block.reserve(1000);
    if(cur.type == TokenType::Symbol && cur.value == "{")
    {
        int depth = 0;
        while (cur.type != TokenType::Eof) {
            if (cur.value == "{") depth++;
            else if (cur.value == "}") {
                depth--;
                if (depth == 0) {
                    block.push_back(std::string(cur.value));
                    advance();
                    break;
                }
            }
            block.push_back(std::string(cur.value));
            advance();
        }
    }
    return block;
}

std::string_view ParserLexem::skip_body() {
    return lexer.skip_block();
}


std::vector<Object> ParserLexem::read_templates()
{
    std::vector<Object> params;

    // если следующий токен — '<', начинаем парсить шаблонные аргументы
    if (cur.type == TokenType::Symbol && cur.value == "<") {
        int angleDepth = 1;
        int parenDepth = 0;
        std::string raw;
        advance();  // съедаем '<'

        // собираем всё до совпадения парных '<' и '>'
        while (cur.type != TokenType::Eof && angleDepth > 0) {
            if (cur.type == TokenType::Symbol) {
                if (cur.value == "<")      { ++angleDepth; raw += cur.value; advance(); }
                else if (cur.value == ">") { --angleDepth; raw += cur.value; advance(); }
                else if (cur.value == "(") { ++parenDepth; raw += cur.value; advance(); }
                else if (cur.value == ")") { --parenDepth; raw += cur.value; advance(); }
                else if (cur.value == ",") { raw += cur.value; advance(); }
                else if (cur.value == "*") { raw += cur.value; advance(); }
                else if (cur.value == ":") {
                    raw += cur.value;
                    advance();
                    assert(cur.type == TokenType::Identifier);
                    raw += cur.value;
                    advance();
                } else {
                    assert(false);
                }
            }
            else {
                raw += " ";
                raw += cur.value;
                advance();
            }
        }
        // raw теперь содержит всё между '<' и '>', включая вложенные скобки

        // разбираем raw на параметры по запятым,
        // игнорируя запятые внутри <…> или (…)
        std::string curr;
        angleDepth = 0;
        parenDepth = 0;
        for (char c : raw) {
            if (c == '<') {
                ++angleDepth;
                curr += c;
            }
            else if (c == '>') {
                --angleDepth;
                curr += c;
            }
            else if (c == '(') {
                ++parenDepth;
                curr += c;
            }
            else if (c == ')') {
                --parenDepth;
                curr += c;
            }
            else if (c == ',' && angleDepth == 0 && parenDepth == 0) {
                // точка разделения
                auto l = curr.find_first_not_of(" \t\r\n");
                auto r = curr.find_last_not_of (" \t\r\n");
                if (l != std::string::npos) {
                    std::string piece = curr.substr(l, r - l + 1);
                    params.push_back(parse_object(piece));
                }
                curr.clear();
            }
            else {
                curr += c;
            }
        }
        // последний параметр
        if (!curr.empty()) {
            auto l = curr.find_first_not_of(" \t\r\n");
            auto r = curr.find_last_not_of (" \t\r\n");
            if (l != std::string::npos) {
                std::string piece = curr.substr(l, r - l + 1);
                params.push_back(parse_object(piece));
            }
        }
    }

    return params;
}

Object ParserLexem::parse_member(bool with_name, bool is_enum) {
    Object member;
    member.type = cur.value;
    advance();
    if (cur.type == TokenType::Symbol && cur.value == "*") {
        member.is_pointer = true;
        advance();
    }

    member.template_args = read_templates();
    member.callable_args = read_callable_args();

    read_modifier(member);
    
    if (!is_enum && with_name && cur.type == TokenType::Identifier) {
        member.name = cur.value;
        advance();
    }
    
    if(is_enum)
        member.name = std::move(member.type);
    
    std::string prefix;
    if (cur.type == TokenType::Symbol && cur.value == "=") {
        advance();
        if (cur.type == TokenType::Symbol && cur.value == "-") {
            prefix += cur.value;
            advance();
        }
        if (cur.type == TokenType::Identifier) {
            if(prefix.empty())
                member.value = std::string(cur.value);
            else
                member.value = prefix + std::string(cur.value);
            advance();
            if(cur.type == TokenType::Symbol && in(cur.value, {"+", "-", "*", "/", ">"}))
            {
                member.value += cur.value;
                advance();
                assert (cur.type == TokenType::Identifier);
                member.value += cur.value;
                advance();
            }
            if(cur.type == TokenType::Symbol && cur.value == ":")
            {
                advance();
                assert(cur.type == TokenType::Symbol && cur.value == ":");
                advance();
                assert(cur.type == TokenType::Identifier);
                member.value += cur.value;
                advance();
            }
        }
    }
    
    return member;
}

void ParserLexem::parse_constructor(Class &cls) {
    Function method;
    assert(cur.value == "constructor");
    advance(); // constructor
    method.callable_args = read_callable_args();
    read_modifier(method);
    method.name = cls.name;
    method.body = skip_body(); // тело метода
    cls.constructors.push_back(std::move(method));
}

Function ParserLexem::parse_method() {
    Function method;
    
    advance(); // fn
    method.template_args = read_templates();
    method.return_type = parse_member(false, false);
    method.name = cur.value;
    advance();
    if(method.name == "operator") {
        while(cur.value != "("){
            method.name += cur.value;
            advance();
        }
    }
    method.callable_args = read_callable_args();
    //TODO: throw error
    assert(cur.value != ";");
    read_modifier(method);
    method.body = skip_body();
    return method;
}

std::vector<Object> ParserLexem::read_callable_args(){
    std::vector<Object> args;
    if (cur.value == "(") {
        advance();
        while (cur.value != ")" && cur.type != TokenType::Eof) {
            if (cur.value == ","){
                advance();
                continue;
            }
            args.push_back(parse_member(true, false));
        }
        assert(cur.value == ")");
        advance(); // ')'
    }
    return args;
}

Object parse_object(const std::string& str, bool with_name)
{
    ParserLexem parser(str);
    auto object = parser.parse_member(with_name, false);
    return object;
}

Function parse_function(const std::string& str)
{
    ParserLexem parser(str);
    auto object = parser.parse_method();
    return object;
    
}
