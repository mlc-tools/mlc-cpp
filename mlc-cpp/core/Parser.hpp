//
//  Parser.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include "Model.hpp"
#include "Object.hpp"
#include "Class.hpp"
#include "Function.hpp"
//#include "Error.hpp"
//#include "Common.hpp"  // parseObject, smartSplit



class Parser {
public:
    explicit Parser(Model &model);
    void parseFiles(const std::vector<std::string> &filePaths);
    void parseText(const std::string &text);
    
    // Helpers for testing
    static Object createObject(const std::string &description);
    static Function createFunction(const std::string &description);
    
    
    static std::string removeComments(const std::string &text);
    static std::pair<bool, std::string> checkSkip(const std::string &line, Model &model);
    void parse_class(const std::shared_ptr<Class>& cls);
    
private:
    Model &model;
};
