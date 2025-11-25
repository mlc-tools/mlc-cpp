//
//  Common.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#ifndef Common_h
#define Common_h

#include <algorithm>
#include <cctype>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

inline std::vector<std::string>
smartSplit(const std::string &str, char divider,
           const std::pair<std::string, std::string> &symbols = {"<(", ">)"}) {
    std::vector<std::string> parts;
    if (str.empty())
        return parts;

    const auto &leftChars = symbols.first;
    const auto &rightChars = symbols.second;
    int depth = 0;
    std::size_t start = 0;

    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str[i];
        if (leftChars.find(c) != std::string::npos) {
            ++depth;
        } else if (rightChars.find(c) != std::string::npos) {
            --depth;
        } else if (c == divider && depth == 0 && i > start) {
            parts.emplace_back(str.substr(start, i - start));
            start = i + 1;
        }
    }
    // последний кусок
    parts.emplace_back(str.substr(start));
    return parts;
}

std::vector<std::string> split(const std::string &values, char delimiter = ',');

inline void replace_all(std::string &s, const std::string &what,
                        const std::string &with) {
    size_t pos = 0;
    while ((pos = s.find(what, pos)) != std::string::npos) {
        s.replace(pos, what.size(), with);
        pos += with.size();
    }
}

template <typename T> std::string to_string(const T &value);

std::string to_string(std::string_view value);
std::string to_string(const char *value);

bool to_bool(const std::string &v, bool *ok = nullptr);

template <typename T>
std::string join(const std::vector<T> &in, const char delimiter = ',') {
    std::string result;
    int index = 0;
    for (auto &t : in) {
        result += to_string(t);
        if (index < (in.size() - 1))
            result.push_back(delimiter);
        ++index;
    }
    return result;
}
template <typename T>
std::string join(const std::vector<T> &in, const std::string &delimiter) {
    std::string result;
    int index = 0;
    for (auto &t : in) {
        result += to_string(t);
        if (index < (in.size() - 1))
            result += delimiter;
        ++index;
    }
    return result;
}

inline std::string to_upper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return s;
}

std::string format(const std::string &template_str,
                   const std::unordered_map<std::string, std::string> &values);

template <typename... Args>
std::string format_indexes(const std::string &template_str, Args &&...args) {
    std::string query = template_str;
    std::tuple<Args...> values(std::forward<Args>(args)...);

    auto replace = [&](int index, const std::string &value) {
        std::string token = "{" + std::to_string(index) + "}";
        size_t pos = 0;
        bool was_replace = false;
        while ((pos = query.find(token, pos)) != std::string::npos) {
            query.replace(pos, token.length(), value);
            pos += value.length();
            was_replace = true;
        }
    };

    std::apply(
        [&](auto &&...unpacked) {
            int i = 0;
            ((replace(i++, to_string(unpacked))), ...);
        },
        values);

    return query;
}

std::string strip(const std::string &s);

bool is_number(const std::string& s);

bool starts_with(const std::string& view, const std::string_view& value);
bool ends_with(const std::string& view, const std::string_view& value);

#endif /* Common_h */
