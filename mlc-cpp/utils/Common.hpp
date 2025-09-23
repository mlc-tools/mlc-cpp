//
//  Common.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#ifndef Common_h
#define Common_h

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cctype>
#include <map>
#include <unordered_map>

inline std::vector<std::string> smartSplit(
    const std::string& str,
    char divider,
    const std::pair<std::string, std::string>& symbols = { "<(", ">)" })
{
    std::vector<std::string> parts;
    if (str.empty()) return parts;

    const auto& leftChars  = symbols.first;
    const auto& rightChars = symbols.second;
    int depth = 0;
    std::size_t start = 0;

    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str[i];
        if (leftChars.find(c)  != std::string::npos) {
            ++depth;
        }
        else if (rightChars.find(c) != std::string::npos) {
            --depth;
        }
        else if (c == divider && depth == 0 && i > start) {
            parts.emplace_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    // последний кусок
    parts.emplace_back(str.substr(start));
    return parts;
}


template <typename T>
std::vector<T> split(const std::string& values, char delimiter = ',')
{
    std::vector<T> out;
    if( !values.empty() )
    {
        std::string string = values;
        do
        {
            size_t k = string.find_first_of( delimiter );
            if( k == -1 )
            {
                out.push_back( strTo<T>(string) );
                break;
            }
            
            out.push_back(strTo<T>(string.substr(0, k)));
            string = string.substr(k + 1);
            if( string.empty() )
                break;
        }
        while( true );
    }
    return out;
}

inline void replace_all(std::string &s, const std::string &what, const std::string &with) {
    size_t pos = 0;
    while ((pos = s.find(what, pos)) != std::string::npos) {
        s.replace(pos, what.size(), with);
        pos += with.size();
    }
}

template <typename T>
std::string to_string(const T& value);

template <typename T>
std::string join( const std::vector<T>& in, const char delimiter = ',')
{
    std::string result;
    int index = 0;
    for( auto& t : in )
    {
        result += to_string(t);
        if(index < (in.size()-1))
            result.push_back( delimiter );
        ++index;
    }
    return result;
}
template <typename T>
std::string join( const std::vector<T>& in, const std::string& delimiter)
{
    std::string result;
    int index = 0;
    for( auto& t : in )
    {
        result += to_string(t);
        if(index < (in.size()-1))
            result += delimiter;
        ++index;
    }
    return result;
}

inline std::string to_upper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); });
    return s;
}

std::string format(const std::string& template_str, const std::unordered_map<std::string, std::string>& values);

template <typename... Args>
std::string format_indexes(const std::string& template_str, Args&&... args) {
    std::string query = template_str;
    std::tuple<Args...> values(std::forward<Args>(args)...);

    auto replace = [&](int index, const std::string& value) {
        std::string token = "{" + std::to_string(index) + "}";
        size_t pos = 0;
        bool was_replace = false;
        while ((pos = query.find(token, pos)) != std::string::npos) {
            query.replace(pos, token.length(), value);
            pos += value.length();
            was_replace = true;
        }
    };

    std::apply([&](auto&&... unpacked) {
        int i = 0;
        ((replace(i++, to_string(unpacked))), ...);
    }, values);
    
    return query;
}

std::string strip(const std::string& s);

#endif /* Common_h */
