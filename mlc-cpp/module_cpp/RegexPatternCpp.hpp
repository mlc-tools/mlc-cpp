//
//  RegexPatternCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//
// RegexPatternCpp.hpp
#pragma once

#include <regex>
#include <string>
#include <vector>
#include <unordered_map>

/// Хранит один шаблон замены: regex, строка-замена и триггеры
struct RegexPattern {
    std::regex             pattern;
    std::string            replacement;
    std::vector<std::string> triggers;
};

class RegexPatternCpp {
public:
    /// Кэш регулярных выражений для имён классов
    static inline std::unordered_map<std::string, std::regex> regsClassNames;

    /// Шаблоны преобразования синтаксиса C++17 → C++14
    static inline std::vector<RegexPattern> convertC17toC14 = {
        {
            std::regex(R"(for\s*\(auto&&\s*\[(\w+),\s*(\w+)\]\s*:\s*(.+)\)\s*\{)"),
            R"(for (auto&& pair : $3)
{
    auto& $1 = pair.first;
    auto& $2 = pair.second;
    (void)$1; //don't generate 'Unused variable' warning
    (void)$2; //don't generate 'Unused variable' warning)",
            { "for" }
        }
    };

    /// Удаление аргументов по умолчанию: `\s*=\s*.+`
    static inline std::regex funcArgsPattern{ R"(\s*=\s*.+)" };
    static inline std::string funcArgsReplacement{ "" };

    /// Шаблоны для пользовательских функций-«макросов»
    static inline std::vector<RegexPattern> functionPatterns = {
        {
            std::regex(R"(map_remove_if\(([\w\d\-\>\.\[\]]+),\s*\((\w+),\s*(\w+)\s*:>\s*(.+)\)\))"),
            R"(for(auto __iter__ = $1.begin(); __iter__ != $1.end();)
{
    auto& $2 = __iter__->first; auto& $3 = __iter__->second; (void)$2;(void)$3;
    if($4) __iter__ = $1.erase(__iter__);
    else ++__iter__;
})",
            { "map_remove_if" }
        },
        {
            std::regex(R"(list_remove_if\(([\w\d\-\>\[\]]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
            R"(auto iter = std::remove_if($1.begin(), $1.end(), [&](const auto& $2)
{
    return $3;
});
$1.erase(iter, $1.end());)",
            { "list_remove_if" }
        },
        {
            std::regex(R"(map_do_if\(([\w\d\-\>\.\[\]]+),\s*\((\w+),\s*(\w+?)\s*:>\s*(.+?):>\s*(.+)\)\);)"),
            R"(for(auto __iter__ = $1.begin(); __iter__ != $1.end();)
{
    auto& $2 = __iter__->first; auto& $3 = __iter__->second; (void)$2;(void)$3;
    if($4) { ++__iter__; $5; }
    else ++__iter__;
})",
            { "map_do_if" }
        },
        {
            std::regex(R"(list_do_if\(([\w\d\-\>\[\]]+),\s*\((\w+)\s*:>\s*(.+?):>\s*(.+)\)\);)"),
            R"(for(int __index__ = 0; __index__ < $1.size(); ++__index__)
{
    auto& $2 = $1.at(__index__);
    if($3)
    {
        auto __size__ = $1.size();
        $4;
        if(__size__ != $1.size()) { --__index__; }
    }
})",
            { "list_do_if" }
        },
        {
            std::regex(R"(list_do\(([\w\d\-\>\.\[\]]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
            R"(for(int __index__ = 0; __index__ < $1.size(); ++__index__)
{
    auto& $2 = $1.at(__index__);
    $3;
})",
            { "list_do" }
        },
        {
            std::regex(R"(throw new Exception\((.*?)\))"),
            "throw std::exception($1);",
            { "throw " }
        },
        {
            std::regex(R"((\w+)\*\s+(\w+)\s*=\s*new\s*(\w+)\s*\(\s*\))"),
            "auto $2 = make_intrusive<$3>();",
            { "new" }
        },
        {
            std::regex(R"(\bnew\s*(\w+)\s*\((.*)\))"),
            "make_intrusive<$1>($2)",
            { "new" }
        },
        {
            std::regex(R"(\blist<([<:>\w\s\*&]+)>\s*(\w+))"),
            "std::vector<$1> $2",
            { "list<" }
        },
        {
            std::regex(R"(\bmap<([<:>\w\s\*&]+),\s*([<:>\w\s\*&]+)>\s*(\w+))"),
            "std::map<$1, $2> $3",
            { "map<" }
        },
        {
            std::regex(R"(std::strcat\((.+?),\s*(.+?)\))"),
            "(std::string($1) + std::string($2))",
            { "std::strcat" }
        },
        {
            std::regex(R"(std::std::)"),
            "std::",
            { "std::std" }
        },
        {
            std::regex(R"(([\w\->\.]+)\s*=\s*([\w\->\.]+)\s*\?\?\s*([\w\->\.]+))"),
            "$1 = ($2 != nullptr) ? $2 : ($3)",
            { "??" }
        },
        {
            std::regex(R"(\bprint\s*\(\s*(.+)\);)"),
            "std::cout << format($1) << std::endl;",
            { "print" }
        },
        {
            std::regex(R"(try\n\s*\{([\s\S]+?)\}\n\s*catch\(((\w+)\s*(\w*))\)\n\s*\{([\s\S]+?)\})"),
            R"(try
{$1}
catch(const std::exception& $4)
{$5})",
            { "try", "catch" }
        }
        ,
        // compound assignment with conditional call shorthand
        {
            std::regex(R"((^|\n)(\s*)([^+\-*/=\n;]*?\b)(\w+)\s*(\+=|\-=|\*=|/=)\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
            std::string("$1$2auto __tmp_$4 = $7;\n") +
            "$1$2if(__tmp_$4)\n" +
            "$1$2{\n" +
            "$1$2    $3$4 $5 $6__tmp_$4->$8;\n" +
            "$1$2}",
            { "?->" }
        }
        ,
        // declaration assignment with conditional call shorthand
        {
            std::regex(R"((^|\n)(\s*)([A-Za-z_][\w:\s<>&\*]*?)\s+(\w+)\s*=\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
            std::string("$1$2auto __tmp_$4 = $6;\n") +
            "$1$2$3 $4;\n" +
            "$1$2if(__tmp_$4)\n" +
            "$1$2{\n" +
            "$1$2    $4 = $5__tmp_$4->$7;\n" +
            "$1$2}",
            { "?->" }
        }
        ,
        // plain assignment with conditional call shorthand
        {
            std::regex(R"((^|\n)(\s*)([^=\n;]*?\b)(\w+)\s*=\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
            std::string("$1$2auto __tmp_$4 = $6;\n") +
            "$1$2if(__tmp_$4)\n" +
            "$1$2{\n" +
            "$1$2    $3$4 = $5__tmp_$4->$7;\n" +
            "$1$2}",
            { "?->" }
        }
        ,
        // bare conditional call shorthand: obj?->call; -> if(obj){ obj->call; }
        {
            std::regex(R"(([\w\-\>\.\:\[\]\(\)]+?)\s*\?->\s*([^;]+);)"),
            std::string("if($1)\n{\n$1->$2;\n}"),
            { "?->" }
        }
    };

    /// Простые текстовые замены
    static inline std::vector<std::pair<std::string,std::string>> replaces = {
        { "std::round", "round" }
    };
};
