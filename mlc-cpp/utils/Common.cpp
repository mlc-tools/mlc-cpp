//
//  Common.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "Common.hpp"


template <> std::string to_string(const int& value) { return std::to_string(value); }
template <> std::string to_string(const unsigned int& value) { return std::to_string(value); }
template <> std::string to_string(const size_t& value) { return std::to_string(value); }
template <> std::string to_string(const float& value) { return std::to_string(value); }
template <> std::string to_string(const std::string& value) { return value; }
template <> std::string to_string(const bool& value) { return value ? "yes" : "no"; }

std::string format(const std::string& template_str, const std::unordered_map<std::string, std::string>& values)
{
    std::string result = template_str;
    for(auto&& [key, value] : values)
    {
        replace_all(result, "{" + key + "}", to_string(value));
    }
    return result;
}
