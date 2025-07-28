//
//  Lexer.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//
#pragma once

#include <string>


enum class TokenType {
    Identifier, // object name, method name, class name, etc
    Symbol,  // {, ;, (, ), etc
    Keyword, // class, constructor, etc
    Include, // list of includes
    Eof
};

struct Token {
    TokenType type;
    std::string_view value;
};

class Lexer {
public:
    explicit Lexer(const std::string &src);
    Token next();

    constexpr void skip_ws();
    constexpr std::string_view read_word() ;
    constexpr std::string_view read_string(char quote) ;
    constexpr bool is_special(char c);
    constexpr bool is_symbol(char c);

    std::string_view skip_block();
private:
    const std::string &text;
    size_t pos;
};
