//
//  Common.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "Common.hpp"

template <> std::string to_string(const int &value) {
    return std::to_string(value);
}
template <> std::string to_string(const unsigned int &value) {
    return std::to_string(value);
}
template <> std::string to_string(const size_t &value) {
    return std::to_string(value);
}
template <> std::string to_string(const float &value) {
    return std::to_string(value);
}
template <> std::string to_string(const std::string &value) { return value; }
template <> std::string to_string(const bool &value) {
    return value ? "yes" : "no";
}
std::string to_string(std::string_view value) { return std::string(value); }
std::string to_string(const char *value) { return std::string(value); }

std::vector<std::string> split(const std::string &values, char delimiter) {
    std::vector<std::string> out;
    if (!values.empty()) {
        std::string string = values;
        do {
            size_t k = string.find_first_of(delimiter);
            if (k == -1) {
                out.push_back(string);
                break;
            }

            out.push_back(string.substr(0, k));
            string = string.substr(k + 1);
            if (string.empty())
                break;
        } while (true);
    }
    return out;
}

bool to_bool(const std::string &v, bool *ok) {
    std::string s;
    s.reserve(v.size());
    for (char ch : v)
        s.push_back(
            static_cast<char>(::tolower(static_cast<unsigned char>(ch))));
    bool val = (s == "1" || s == "true" || s == "yes" || s == "on");
    if (ok)
        *ok = (s == "1" || s == "0" || s == "true" || s == "false" ||
               s == "yes" || s == "no" || s == "on" || s == "off");
    if (s == "0" || s == "false" || s == "no" || s == "off")
        val = false;
    return val;
};

std::string format(const std::string &template_str,
                   const std::unordered_map<std::string, std::string> &values) {
    std::string result = template_str;
    for (auto &&[key, value] : values) {
        replace_all(result, "{" + key + "}", to_string(value));
    }
    return result;
}

std::string strip(const std::string &s) {
    // trim left
    size_t start = 0;
    while (start < s.size() &&
           std::isspace(static_cast<unsigned char>(s[start])))
        ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;
    return s.substr(start, end - start);
}
