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
#include "ParserLexem.hpp"




// --- Парсер ---




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
    
    std::vector<std::shared_ptr<Class>> inner_classes;
    for(auto& cls : classes){
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
    
    model.classes.insert(model.classes.end(), classes.begin(), classes.end());
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

