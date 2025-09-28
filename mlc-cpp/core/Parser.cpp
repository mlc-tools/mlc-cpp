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




Parser::Parser(Model &m) : _model(m) {}

void Parser::parseFiles(const std::vector<std::string> &filePaths) {
    for (const auto &path : filePaths) {
        std::ifstream ifs(path);
        std::stringstream buf;
        buf << ifs.rdbuf();
        _current_source_path = path;
        parseText(buf.str());
    }
}

void Parser::parseText(const std::string &input) {
    std::string text = removeComments(input);
    ParserLexem parser(text, _model);
    auto classes = parser.parse_dict();
    
    std::vector<std::shared_ptr<Class>> inner_classes;
    for(auto& cls : classes){
        parse_class(cls);
        for(auto& inner : cls->inner_classes){
            for(auto& m : cls->members){
                if(m.type == inner->name) m.type = cls->name + inner->name;
                for(auto& t : m.template_args){
                    if(t.type == inner->name) t.type = cls->name + inner->name;
                }
                for(auto& t : m.callable_args){
                    if(t.type == inner->name) t.type = cls->name + inner->name;
                }
            }
            for(auto& m : cls->functions){
                if(m.return_type.type == inner->name) m.return_type.type = cls->name + inner->name;
                for(auto& t : m.template_args){
                    if(t.type == inner->name) t.type = cls->name + inner->name;
                }
                for(auto& t : m.callable_args){
                    if(t.type == inner->name) t.type = cls->name + inner->name;
                }
            }
            inner->name = cls->name + inner->name;
            inner->group = cls->group;
        }
        inner_classes.insert(inner_classes.end(), cls->inner_classes.begin(), cls->inner_classes.end());
    }
    
    for(auto& cls : inner_classes){
        parse_class(cls);
        assert(cls->inner_classes.empty());
    }
    
    // проставим исходный файл
    for (auto &c : classes)
        c->source_path = _current_source_path;
    for (auto &c : inner_classes)
        c->source_path = _current_source_path;
    _model.add_classes(classes);
    _model.add_classes(inner_classes);
}

void Parser::parse_class(const std::shared_ptr<Class>& cls)
{
    ParserLexem parser(cls->inner_body, _model);
    parser.parse(cls);
    cls->generate_constructor();
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
