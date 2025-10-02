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
    if (pos >= text.size())
        return {TokenType::Eof, ""};

    char c = text[pos];

    // include (@include)
    if (c == '@') {
        return {TokenType::Include, read_word()};
    }

    // символы
    else if (c == ':' && text[pos + 1] == ':') {
        Token token = {TokenType::Symbol, std::string_view(&text[pos], 2)};
        pos += 2;
        return token;
    } else if (is_symbol(c) || (is_special(c) && c != '_')) {
        return {TokenType::Symbol, std::string_view(&text[pos++], 1)};
    }
    // слово
    else if (std::isalnum(c) || c == '_' || c == ':' || c == '/') {
        auto word = read_word();
        if (word == "class" || word == "fn" || word == "function" ||
            word == "constructor" || word == "enum" || word == "interface") {
            if (word == "function")
                word = "fn";
            return {TokenType::Keyword, word};
        }
        return {TokenType::Identifier, word};
    } else if (c == '"' || c == '\'') {
        auto string = read_string(c);
        return {TokenType::Identifier, string};
    } else if (c == '-' && text[pos + 1] == '>') {
        pos += 2;
        return {TokenType::Symbol, "->"};
    } else {
        std::cout << "Skip read char: [" << c << "]." << std::endl;
    }

    pos++;
    return next(); // пропускаем неизвестное
}

constexpr void Lexer::skip_ws() {
    while (pos < text.size() && std::isspace(text[pos]))
        ++pos;
}
std::string_view Lexer::skip_block() {
    int depth = 1;
    bool in_string = false;
    size_t start = pos;
    while (pos < text.size()) {
        if (!in_string && text[pos] == '{')
            ++depth;
        if (!in_string && text[pos] == '}' && --depth == 0) {
            break;
        };
        if (!in_string && text[pos] == '"')
            in_string = true;
        if (in_string && text[pos] == '"' && text[pos - 1] != '\\')
            in_string = false;
        ++pos;
    }
    std::string_view result;
    if (pos > start)
        result = std::string_view(&text[start], pos - start);
    if (pos < text.size())
        ++pos;
    return result;
}
constexpr std::string_view Lexer::read_word() {
    size_t start = pos;
    while (pos < text.size() &&
           (std::isalnum(text[pos]) || is_special(text[pos])))
        pos++;
    return std::string_view(&text[start], pos - start);
}
constexpr std::string_view Lexer::read_string(char quote) {
    size_t start = pos;
    pos++;
    while (pos < text.size() && (text[pos] != quote || text[pos - 1] == '\\'))
        pos++;
    if (pos < text.size())
        pos++;
    return std::string_view(&text[start], pos - start);
}
constexpr bool Lexer::is_special(char c) {
    return std::string("_@&[]!|.#?").find(c) != std::string::npos;
}
constexpr bool Lexer::is_symbol(char c) {
    return c == '{' || c == '}' || c == '(' || c == ')' || c == ';' ||
           c == '.' || c == ',' || c == ':' || c == '<' || c == '>' ||
           c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
           c == '%' || false;
}

std::string_view Lexer::get_current_line() const {
    size_t start = this->pos;
    size_t end = this->pos;
    while (--start >= 0) {
        if (text[start] == '\n')
            break;
    }
    while (end < text.size()) {
        if (text[end] == '\n')
            break;
        ++end;
    }
    return std::string_view(&text[start + 1], end - start);
}
