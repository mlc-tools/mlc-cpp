//
//  Parser.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Parser.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cassert>

// Initialize static regex



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

Object parse_object(const std::string& str);

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

private:
    const std::string &text;
    size_t pos;

    constexpr void skip_ws() {
        while (pos < text.size() && std::isspace(text[pos]))
            ++pos;
    }
public:
    std::string skip_block(){
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
private:
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
        return std::string_view("_@*&[]+!|.#?-").find(c) != std::string_view::npos;
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
        c == '<' ||
        c == '>' ||
        c == ',' ||
        c == '/' ||
        false
        ;
    }
};

// --- Парсер ---
class ParserLexem {
public:
    explicit ParserLexem(const std::string &src) : lexer(src) {
        advance();
    }
    
    void read_class_name_and_group(std::shared_ptr<Class>& cls)
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
    void read_modifier(Object& object)
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
    void read_parent_class(std::shared_ptr<Class>& cls){
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
    
    std::vector<std::shared_ptr<Class>> parse_dict(bool one=false) {
        std::vector<std::shared_ptr<Class>> result;
        // ищем class
        while (cur.type != TokenType::Eof) {
//            if (cur.type == TokenType::Keyword && cur.value == "enum") {
//                advance();
//
//                result.push_back(std::make_shared<Class>());
//                auto cls = result.back();
//                cls->is_enum = true;
//                read_class_name_and_group(cls);
//                read_modifier(*cls);
//                if(cur.type == TokenType::Symbol && cur.value == "{")
//                {
//                    advance();
//                    do
//                    {
//                        if(cur.type == TokenType::Identifier){
//                            Object object;
//                            object.name = std::move(cur.value);
//                            cls->members.push_back(std::move(object));
//                            advance();
//                        }
//                    }
//                    while(cur.type != TokenType::Symbol || cur.value != "}");
//                    advance();
//                }
//                
//            }
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
    
    void parse(const std::shared_ptr<Class>& cls) {
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
    
private:
    Lexer lexer;
    Token cur;
    
    void advance() { cur = lexer.next(); }
    
    std::vector<std::string> read_body() {
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
    
    std::string skip_body() {
        return lexer.skip_block();
    }
    
    std::string read_until(const std::set<std::string>& symbols){
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
    
    std::vector<Object> read_templates()
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
public:
    Object parse_member(bool with_name, bool is_enum) {
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
    
    void parse_constructor(Class &cls) {
        Function method;
        assert(cur.value == "constructor");
        advance(); // constructor
        method.args = parse_method_args();
        read_modifier(method);
        method.name = cls.name;
        method.body = read_body(); // тело метода
        cls.constructors.push_back(std::move(method));
    }
    
    void parse_method(Class &cls) {
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
    
    std::vector<Object> parse_method_args(){
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
};


Parser::Parser(Model &m) : model(m) {}

void Parser::parseFiles(const std::vector<std::string> &filePaths) {
    for (const auto &path : filePaths) {
        std::ifstream ifs(path);
        std::stringstream buf;
        buf << ifs.rdbuf();
        parseText(buf.str());
    }
}

void Parser::parseText(const std::string &input) {
    std::string text = removeComments(input);
    ParserLexem parser(text);
    auto classes = parser.parse_dict();
    model.classes.insert(model.classes.end(), classes.begin(), classes.end());
    
    std::vector<std::shared_ptr<Class>> inner_classes;
    for(auto& cls : model.classes){
        parse_class(cls);
        for(auto& inner : cls->inner_classes){
            inner->name = cls->name + inner->name;
        }
        inner_classes.insert(inner_classes.end(), cls->inner_classes.begin(), cls->inner_classes.end());
    }
    
    for(auto& cls : inner_classes){
        parse_class(cls);
        assert(cls->inner_classes.empty());
    }
    model.classes.insert(model.classes.end(), inner_classes.begin(), inner_classes.end());
}

void Parser::parse_class(const std::shared_ptr<Class>& cls)
{
    ParserLexem parser(cls->inner_body);
    parser.parse(cls);
}

std::string Parser::removeComments(const std::string &txt) {
    std::string s = txt;
    // remove /* */
    size_t p;
    while ((p = s.find("/*")) != std::string::npos) {
        size_t q = s.find("*/", p + 2);
        if (q != std::string::npos) s.erase(p, q - p + 2);
        else break;
    }
    // remove //
    std::istringstream iss(s);
    std::string line, out;
    while (std::getline(iss, line)) {
        auto cpos = line.find("//");
        if (cpos != std::string::npos) line.erase(cpos);
        out += line + '\n';
    }
    return out;
}

std::pair<bool, std::string> Parser::checkSkip(const std::string &ln, Model &m) {
    bool corresponds = true;
    bool firstMatch = true;
    std::string line = ln;
    for (auto lang : {"cpp", "py", "php", "js"}) {
        std::string mod = std::string(":") + lang;
        size_t pos;
        if ((pos = line.find(mod)) != std::string::npos) {
            if (firstMatch) {
                firstMatch = false;
                corresponds = false;
            }
            corresponds = corresponds || m.isLang(lang);
            line.erase(pos, mod.size());
        }
    }
    return { !corresponds, line };
}

Object parse_object(const std::string& str)
{
    ParserLexem parser(str);
    auto object = parser.parse_member(false, false);
    return object;
}
