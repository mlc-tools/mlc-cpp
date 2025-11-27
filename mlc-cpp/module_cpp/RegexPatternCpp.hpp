//
//  RegexPatternCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//
// RegexPatternCpp.hpp
#pragma once

#include <memory>
#include <re2/re2.h>
#include <string>
#include <unordered_map>
#include <vector>

/// Хранит один шаблон замены: regex, строка-замена и триггеры
struct RegexPattern {
    std::unique_ptr<RE2> pattern;
    std::string replacement;
    std::vector<std::string> triggers;
};

class RegexPatternCpp {
  public:
    /// Кэш регулярных выражений для имён классов
    static inline std::unordered_map<std::string, std::unique_ptr<RE2>>
        regsClassNames;

    /// Шаблоны преобразования синтаксиса C++17 → C++14
    static inline std::vector<RegexPattern> convertC17toC14 = [] {
        std::vector<RegexPattern> v;
        v.push_back(
            {std::make_unique<RE2>(
                 R"(for\s*\(auto&&\s*\[(\w+),\s*(\w+)\]\s*:\s*(.+)\)\s*\{)"),
             R"(for (auto&& pair : \3)
{
    auto& \1 = pair.first;
    auto& \2 = pair.second;
    (void)\1; //don't generate 'Unused variable' warning
    (void)\2; //don't generate 'Unused variable' warning)",
             {"for"}});
        return v;
    }();

    /// Удаление аргументов по умолчанию: `\s*=\s*.+`
    //    static inline RE2 funcArgsPattern(R"(\s*=\s*.+)");
    //    static inline std::string funcArgsReplacement("");

    /// Шаблоны для пользовательских функций-«макросов»
    static inline std::vector<RegexPattern> functionPatterns = [] {
        std::vector<RegexPattern> v;
        v.push_back(
            {std::make_unique<RE2>(
                 R"(map_remove_if\(([\w\d\-\>\.\[\]\(\)<>]+),\s*\((\w+),\s*(\w+)\s*:>\s*(.+)\)\))"),
             R"(for(auto __iter__ = \1.begin(); __iter__ != \1.end();)
{
    auto& \2 = __iter__->first; auto& \3 = __iter__->second; (void)\2;(void)\3;
    if(\4) __iter__ = \1.erase(__iter__);
    else ++__iter__;
})",
             {"map_remove_if"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(list_remove_if\(([\w\d\-\>\.\[\]\(\)<>]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
             R"(auto iter = std::remove_if(\1.begin(), \1.end(), [&](const auto& \2)
{
    return \3;
});
\1.erase(iter, \1.end());)",
             {"list_remove_if"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(map_do_if\(([\w\d\-\>\.\[\]\(\)<>]+),\s*\((\w+),\s*(\w+?)\s*:>\s*(.+?):>\s*(.+)\)\);)"),
             R"(for(auto __iter__ = \1.begin(); __iter__ != \1.end();)
{
    auto& \2 = __iter__->first; auto& \3 = __iter__->second; (void)\2;(void)\3;
    if(\4) { ++__iter__; \5; }
    else ++__iter__;
})",
             {"map_do_if"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(list_do_if\(([\w\d\-\>\.\[\]\(\)<>]+),\s*\((\w+)\s*:>\s*(.+?):>\s*(.+)\)\);)"),
             R"(for(int __index__ = 0; __index__ < \1.size(); ++__index__)
{
    auto& \2 = \1.at(__index__);
    if(\3)
    {
        auto __size__ = \1.size();
        \4;
        if(__size__ != \1.size()) { --__index__; }
    }
})",
             {"list_do_if"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(list_do\(([\w\d\-\>\.\[\]\(\)<>]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
             R"(for(int __index__ = 0; __index__ < \1.size(); ++__index__)
{
    auto& \2 = \1.at(__index__);
    \3;
})",
             {"list_do"}});
        v.push_back({std::make_unique<RE2>(R"(throw new Exception\((.*?)\))"),
                     R"(throw std::exception(\1);)",
                     {"throw "}});
        v.push_back({std::make_unique<RE2>(R"(\bnew\s*(\w+)\s*\((.*)\))"),
                     R"(make_intrusive<\1>(\2))",
                     {"new"}});
        v.push_back({std::make_unique<RE2>(
                         R"((\w+)\*\s+(\w+)\s*=\s*new\s*(\w+)\s*\(\s*\))"),
                     R"(auto \2 = make_intrusive<\3>();)",
                     {"new"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\blist<([<:>\w\s\*&]+)>\s*(\w+))"),
             R"(std::vector<\1> \2)",
             {"list<"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\bmap<([<:>\w\s\*&]+),\s*([<:>\w\s\*&]+)>\s*(\w+))"),
             R"(std::map<\1, \2> \3)",
             {"map<"}});
        v.push_back({std::make_unique<RE2>(R"(std::strcat\((.+?),\s*(.+?)\))"),
                     R"((std::string(\1) + std::string(\2)))",
                     {"std::strcat"}});
        v.push_back(
            {std::make_unique<RE2>(R"(std::std::)"), "std::", {"std::std"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(([\w\->\.]+)\s*=\s*([\w\->\.]+)\s*\?\?\s*([\w\->\.]+))"),
             R"(\1 = (\2 != nullptr) ? \2 : (\3))",
             {"??"}});
        v.push_back({std::make_unique<RE2>(R"(\bprint\s*\(\s*(.+)\);)"),
                     R"(std::cout << format(\1) << std::endl;)",
                     {"print"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(try\n\s*\{([\s\S]+?)\}\n\s*catch\(((\w+)\s*(\w*))\)\n\s*\{([\s\S]+?)\})"),
             R"(try
{\1}
catch(const std::exception& \4)
{\5})",
             {"try", "catch"}});
        // compound assignment with conditional call shorthand
        v.push_back(
            {std::make_unique<RE2>(
                 R"((^|\n)(\s*)([^+\-*/=\n;]*?\b)(\w+)\s*(\+=|\-=|\*=|/=)\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
             R"__(\1\2auto __tmp_\4 = \7;
            \1\2if(__tmp_\4)
            \1\2{
            \1\2    \3\4 \5 \6__tmp_\4->\8;
            \1\2})__",
             {"?->"}});
        // declaration assignment with conditional call shorthand
        v.push_back(
            {std::make_unique<RE2>(
                 R"((^|\n)(\s*)([A-Za-z_][\w:\s<>&\*]*?)\s+(\w+)\s*=\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
             R"__(\1\2auto __tmp_\4 = \6;
            \1\2\3 \4;
            \1\2if(__tmp_\4)
            \1\2{
            \1\2    \4 = \5__tmp_\4->\7;
            \1\2})__",
             {"?->"}});
        // plain assignment with conditional call shorthand
        v.push_back(
            {std::make_unique<RE2>(
                 R"((^|\n)(\s*)([^=\n;]*?\b)(\w+)\s*=\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
             R"__(\1\2auto __tmp_\4 = \6;
            \1\2if(__tmp_\4)
            \1\2{
            \1\2    \3\4 = \5__tmp_\4->\7;
            \1\2})__",
             {"?->"}});
        // bare conditional call shorthand: obj?->call; -> if(obj){ obj->call; }
        // TODO:
        v.push_back({std::make_unique<RE2>(
                         R"(([\w\-\>\.\:\[\]\(\)]+?)\s*\?->\s*(.+?);)"),
                     R"__(if(\1)
            {
                \1->\2;
            })__",
                     {"?->"}});
        
        v.push_back({
            std::make_unique<RE2>(R"_(auto\s(\w+)\s*,\s*(\w+)\s*=\s*([\w.\->_]+)\.at\(([\w]+)\))_"),
            R"_(auto [\1, \2] = safe_at_value(\3, \4);)_",
            {"at"}});
        v.push_back({
            std::make_unique<RE2>(R"_(auto\s*&\s*(\w+)\s*,\s*(\w+)\s*=\s*([\w.\->_]+)\.at\(([\w]+)\))_"),
            R"_(auto&& [\1, \2] = safe_at_ref(\3, \4);)_",
            {"at"}});

        return v;
    }();

    /// Простые текстовые замены
    static inline std::vector<std::pair<std::string, std::string>> replaces = {
        {"std::round", "round"}};
};
