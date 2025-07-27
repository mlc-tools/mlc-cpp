//
//  parser.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#ifndef parser_hpp
#define parser_hpp

#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Class.hpp"

std::vector<Class> parse_class(const std::string &code);

#endif /* parser_hpp */
