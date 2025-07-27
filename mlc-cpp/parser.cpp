//
//  parser.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "parser.hpp"
#include <cctype>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <cassert>

enum class TokenType {
    Identifier, // object name, method name, class name, etc
    Symbol,  // {, ;, (, ), etc
    Keyword, // class, constructor, etc
    Include, // list of includes
    Eof
};

struct Token {
    TokenType type;
    std::string value;
};

// --- Лексер ---
class Lexer {
public:
    explicit Lexer(const std::string &src) : text(src), pos(0) {}

    Token next() {
        skip_ws();
        if (pos >= text.size()) return {TokenType::Eof, ""};

        char c = text[pos];

        // include (@include)
        if (c == '@') {
            std::string word = read_word();
            return {TokenType::Include, word};
        }

        // символы
        else if (is_symbol(c) || is_special(c)) {
            pos++;
            return {TokenType::Symbol, std::string(1, c)};
        }

        // слово
        else  if (std::isalnum(c) || c == '_' || c == ':' || c == '/') {
            std::string word = read_word();
            if (word == "class" || word == "fn" || word == "constructor") {
                return {TokenType::Keyword, word};
            }
            return {TokenType::Identifier, word};
        }
        else if (c == '"' || c == '\'') {
            auto string = read_string(c);
            return {TokenType::Identifier, string};
        }
        else if(c == '-' && text[pos+1] == '>'){
            pos += 2;
            return {TokenType::Symbol, "->"};
        }
        else
        {
            std::cout << "Skip read char: [" << c << "]." << std::endl;
        }

        pos++;
        return next(); // пропускаем неизвестное
    }

private:
    const std::string &text;
    size_t pos;

    constexpr void skip_ws() {
        while (pos < text.size() && std::isspace(text[pos])) pos++;
    }
    constexpr std::string read_word() {
        size_t start = pos;
        while (pos < text.size() && (std::isalnum(text[pos]) || is_special(text[pos])))
            pos++;
        return text.substr(start, pos - start);
    }
    constexpr std::string read_string(char quote) {
        std::string lit = std::string(1, quote);
        pos++;
        while (pos < text.size() && (text[pos] != quote || text[pos-1] == '\\'))
            lit += text[pos++];
        if (pos < text.size())
            lit += text[pos++];
        return lit;
    }
    constexpr bool is_special(char c) {
        return std::string_view("_/@*<>&,[]+!|.").find(c) != std::string_view::npos;
    }
    constexpr bool is_symbol(char c) {
        return
        c == '{' ||
        c == '}' ||
        c == '(' ||
        c == ')' ||
        c == ';' ||
        c == '.' ||
        c == ',' ||
        c == '=' ||
        c == ':' ||
        c == '-' ||
        false
        ;
    }
};

// --- Парсер ---
class Parser {
public:
    explicit Parser(const std::string &src) : lexer(src) {
        advance();
    }

    std::vector<Class> parse() {
        std::vector<Class> result;
        
        int depth = 0;
        // ищем class
        while (cur.type != TokenType::Eof) {
            if (cur.type == TokenType::Keyword && cur.value == "class") {
                result.emplace_back();
                advance();
                // имя (group/className)
                if (cur.type == TokenType::Identifier) {
                    std::string full = cur.value;
                    size_t slash = full.find('/');
                    if (slash != std::string::npos) {
                        result.back().group = full.substr(0, slash);
                        result.back().name = full.substr(slash + 1);
                    } else {
                        result.back().name = full;
                    }
                    advance();
                }
                if(cur.type == TokenType::Symbol && cur.value == ":") {
                    advance();
                    if (cur.type == TokenType::Identifier) {
                        result.back().set_modifier(cur.value);
                        advance();
                    }
                    else {
                        assert(false);
                    }
                }
            } else if (cur.type == TokenType::Include) {
                advance();
                if (cur.type == TokenType::Identifier) {
                    result.back().includes.push_back(cur.value);
                    advance();
                }
            } else if (cur.type == TokenType::Keyword && cur.value == "constructor") {
                parse_constructor(result.back()); // пока игнорируем тело конструктора
            } else if (cur.type == TokenType::Keyword && cur.value == "fn") {
                parse_method(result.back());
            } else if (cur.type == TokenType::Identifier) {
                result.back().members.push_back(parse_member(true));
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

        return result;
    }

private:
    Lexer lexer;
    Token cur;

    void advance() { cur = lexer.next(); }

    std::vector<std::string> read_body() {
        std::vector<std::string> block;
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
        return block;
    }

    Member parse_member(bool with_name) {
        Member member;
        member.type = std::move(cur.value);
        advance();
        if (cur.type == TokenType::Symbol && cur.value == ":") {
            advance();
            if (cur.type == TokenType::Identifier){
                member.set_modifier(cur.value);
                advance();
            }
        }
        if (with_name && cur.type == TokenType::Identifier) {
            member.name = std::move(cur.value);
            std::string prefix;
            advance();
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
                }
            }
        }
        return member;
    }
    
    void parse_constructor(Class &cls) {
        Method method;
        assert(cur.value == "constructor");
        advance(); // constructor
        method.args = parse_method_args();
        method.name = cls.name;
        method.body = read_body(); // тело метода
        cls.constructors.push_back(std::move(method));
    }

    void parse_method(Class &cls) {
        Method method;
        
        advance(); // fn
        method.return_type = parse_member(false);
        method.name = std::move(cur.value);
        advance();
        method.args = parse_method_args();
        if(cur.type == TokenType::Symbol && cur.value == ":"){
            advance();
            if(cur.type == TokenType::Identifier){
                method.set_modifier(cur.value);
                advance();
            }
        }
        method.body = read_body();
        cls.methods.push_back(std::move(method));
    }
    
    std::vector<Member> parse_method_args(){
        std::vector<Member> args;
        if (cur.value == "(") {
            advance();
            while (cur.value != ")" && cur.type != TokenType::Eof) {
                args.push_back(parse_member(true));
            }
            assert(cur.value == ")");
            advance(); // ')'
        }
        return args;
    }
};

std::vector<Class> parse_class(const std::string &code) {
    Parser p(code);
    return p.parse();
}
