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

ParserLexem::ParserLexem(const std::string &src) : lexer(src) {
    advance();
}

void ParserLexem::read_class_name_and_group(std::shared_ptr<Class>& cls)
{
    if (cur.type == TokenType::Identifier) {
        cls->name = std::move(cur.value);
        advance();
        if(cur.type == TokenType::Symbol && cur.value == "/") {
            cls->group = std::move(cls->name);
            advance();
            assert(cur.type == TokenType::Identifier);
            cls->name = std::move(cur.value);
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
            cls->parent_class_name = std::move(cur.value);
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
        if (cur.type == TokenType::Keyword && cur.value == "class") {
            auto inner = parse_dict(true);
            cls->inner_classes.insert(cls->inner_classes.end(), inner.begin(), inner.end());
        }
        else if (cur.type == TokenType::Include) {
            advance();
            if (cur.type == TokenType::Identifier) {
                cls->includes.push_back(cur.value);
                advance();
            }
        } else if (cur.type == TokenType::Keyword && cur.value == "constructor") {
            parse_constructor(*cls); // пока игнорируем тело конструктора
        } else if (cur.type == TokenType::Keyword && cur.value == "fn") {
            parse_method(*cls);
        } else if (cur.type == TokenType::Identifier) {
            cls->members.push_back(parse_member(true, cls->is_enum));
        } else if (cur.type == TokenType::Symbol && cur.value == "{") {
            ++depth;
            advance();
        } else if (cur.type == TokenType::Symbol && cur.value == "}") {
            --depth;
            advance();
            assert(depth >= 0);
        } else {
            std::cout << "Skip: " << cur.value << std::endl;
            advance();
        }
    }
}

void ParserLexem::advance() { cur = lexer.next(); }

std::vector<std::string> ParserLexem::read_body() {
    std::vector<std::string> block;
    if(cur.type == TokenType::Symbol && cur.value == "{")
    {
        int depth = 0;
        while (cur.type != TokenType::Eof) {
            if (cur.value == "{") depth++;
            else if (cur.value == "}") {
                depth--;
                if (depth == 0) {
                    block.push_back(std::move(cur.value));
                    advance();
                    break;
                }
            }
            block.push_back(std::move(cur.value));
            advance();
        }
    }
    return block;
}

std::string ParserLexem::skip_body() {
    return lexer.skip_block();
}

std::string ParserLexem::read_until(const std::set<std::string>& symbols){
    std::string result;
    while(cur.type != TokenType::Eof)
    {
        if(cur.type == TokenType::Symbol && symbols.count(cur.value) > 0)
            return result;
        result += cur.value;
        advance();
    }
    return result;
}

std::vector<Object> ParserLexem::read_templates()
{
    std::vector<Object> params;
    // если следующий токен — '<', начинаем парсить шаблонные аргументы
    if (cur.type == TokenType::Symbol && cur.value == "<") {
        int depth = 1;
        std::string raw;
        advance();  // переходим за '<'
        
        // собираем всё до совпадения парных '<' и '>'
        while (cur.type != TokenType::Eof && depth > 0) {
            if (cur.type == TokenType::Symbol) {
                if (cur.value == "<") depth++;
                else if (cur.value == ">") depth--;
            }
            raw += cur.value;
            advance();
        }
        // raw теперь содержит всё между '<' и '>', включая вложенные
        
        // разбираем raw на отдельные параметры по запятым, учитывая вложенность
        
        std::string curr;
        int d = 0;
        for (char c : raw) {
            if (c == '<') {
                d++;  curr += c;
            } else if (c == '>') {
                d--;  curr += c;
            } else if (c == ',' && d == 0) {
                // сбросить и сохранить
                auto l = curr.find_first_not_of(" \t\n\r");
                auto r = curr.find_last_not_of(" \t\n\r");
                if (l != std::string::npos)
                    params.push_back(parse_object(curr.substr(l, r - l + 1)));
                curr.clear();
            } else {
                curr += c;
            }
        }
        // последний параметр
        if (!curr.empty()) {
            auto l = curr.find_first_not_of(" \t\n\r");
            auto r = curr.find_last_not_of(" \t\n\r");
            if (l != std::string::npos)
                params.push_back(parse_object(curr.substr(l, r - l + 1)));
        }
    }
    return params;
}

Object ParserLexem::parse_member(bool with_name, bool is_enum) {
    Object member;
    member.type = std::move(cur.value);
    advance();
    member.template_args = read_templates();
    read_modifier(member);
    
    if (!is_enum && with_name && cur.type == TokenType::Identifier) {
        member.name = std::move(cur.value);
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
                member.value = std::move(cur.value);
            else
                member.value = prefix + cur.value;
            advance();
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
    method.args = parse_method_args();
    read_modifier(method);
    method.name = cls.name;
    method.body = read_body(); // тело метода
    cls.constructors.push_back(std::move(method));
}

void ParserLexem::parse_method(Class &cls) {
    Function method;
    
    advance(); // fn
    method.template_args = read_templates();
    method.return_type = parse_member(false, cls.is_enum);
    method.name = std::move(cur.value);
    advance();
    if(method.name == "operator") {
        while(cur.value != "("){
            method.name += cur.value;
            advance();
        }
    }
    method.args = parse_method_args();
    read_modifier(method);
    method.body = read_body();
    cls.functions.push_back(std::move(method));
}

std::vector<Object> ParserLexem::parse_method_args(){
    std::vector<Object> args;
    if (cur.value == "(") {
        advance();
        while (cur.value != ")" && cur.type != TokenType::Eof) {
            args.push_back(parse_member(true, false));
        }
        assert(cur.value == ")");
        advance(); // ')'
    }
    return args;
}

Object parse_object(const std::string& str)
{
    ParserLexem parser(str);
    auto object = parser.parse_member(false, false);
    return object;
}
