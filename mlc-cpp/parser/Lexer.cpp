//
//  Lexer.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#include "Lexer.hpp"
#include <iostream>


Lexer::Lexer(const std::string &src) : text(src), pos(0) {}

Token Lexer::next() {
    skip_ws();
    if (pos >= text.size()) return {TokenType::Eof, ""};

    char c = text[pos];

    // include (@include)
    if (c == '@') {
        std::string word = read_word();
        return {TokenType::Include, word};
    }

    // символы
    else if (is_symbol(c) || (is_special(c) && c != '_')) {
        pos++;
        return {TokenType::Symbol, std::string(1, c)};
    }

    // слово
    else  if (std::isalnum(c) || c == '_' || c == ':' || c == '/') {
        std::string word = read_word();
        if (word == "class" || word == "fn" || word == "function" || word == "constructor" || word == "enum" || word == "interface") {
            if(word == "function")
                word = "fn";
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

constexpr void Lexer::skip_ws() {
    while (pos < text.size() && std::isspace(text[pos]))
        ++pos;
}
std::string Lexer::skip_block(){
    int depth = 1;
    bool in_string = false;
    std::string result;
    size_t begin = pos;
    while(pos < text.size()){
        if(!in_string && text[pos] == '{') ++depth;
        if(!in_string && text[pos] == '}' && --depth == 0) {
            break;
        };
        if(!in_string && text[pos] == '"')
            in_string = true;
        if(in_string && text[pos] == '"' && text[pos-1] != '\\')
            in_string = false;
        ++pos;
    }
    result = text.substr(begin, pos-begin);
    ++pos;
    return result;
}
constexpr std::string Lexer::read_word() {
    size_t start = pos;
    while (pos < text.size() && (std::isalnum(text[pos]) || is_special(text[pos])))
        pos++;
    return text.substr(start, pos - start);
}
constexpr std::string Lexer::read_string(char quote) {
    std::string lit = std::string(1, quote);
    pos++;
    while (pos < text.size() && (text[pos] != quote || text[pos-1] == '\\'))
        lit += text[pos++];
    if (pos < text.size())
        lit += text[pos++];
    return lit;
}
constexpr bool Lexer::is_special(char c) {
    return std::string_view("_@&[]!|.#?").find(c) != std::string_view::npos;
}
constexpr bool Lexer::is_symbol(char c) {
    return
    c == '{' ||
    c == '}' ||
    c == '(' ||
    c == ')' ||
    c == ';' ||
    c == '.' ||
    c == ',' ||
    c == ':' ||
    c == '<' ||
    c == '>' ||
    c == '+' ||
    c == '-' ||
    c == '*' ||
    c == '/' ||
    c == '=' ||
    false
    ;
}
