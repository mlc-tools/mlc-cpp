//
//  regex.hpp (Python module)
//  mlc-cpp
//
//  Port of RegexPatternPython from legacy Python to C++ (RE2),
//  using the same pattern container format as module_cpp.
//

#pragma once

#include <memory>
#include <re2/re2.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "../module_cpp/RegexPatternCpp.hpp" // uses RegexPattern struct

class RegexPatternPython {
  public:
    static inline std::unordered_map<std::string, std::unique_ptr<RE2>>
        regsClassNames;

    static inline std::unique_ptr<RE2> FACTORY =
        std::make_unique<RE2>(R"(\bFactory\b)");

    // Function-level regex transformations (pattern, replacement, triggers)
    static inline std::vector<RegexPattern> functionPatterns = [] {
        std::vector<RegexPattern> v;
        // compound assignment with conditional call
        v.push_back(
            {std::make_unique<RE2>(
                 R"((?m)^(\s*)(?:[^+\-*/=\n;]*?\b)?(\w+)\s*(\+=|\-=|\*=|/=)\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
             R"(\1__tmp_\2 = \5
\1if(__tmp_\2)
{
\1\2 \3 \4__tmp_\2->\6;
})",
             {"?->"}});
        // plain assignment with conditional call shorthand
        v.push_back(
            {std::make_unique<RE2>(
                 R"((?m)^(\s*)(?:[^=\n;]*?\b)?(\w+)\s*=\s*(.*?)([\w:\.\-\>\(\)<> ,\*]+)\s*\?->\s*([^;\n]+);)"),
             R"(\1__tmp_\2 = \4
\1\2 = None
\1if(__tmp_\2)
{
\1\2 = \3__tmp_\2->\5;
})",
             {"?->"}});
        // statement form: condition?->action();
        v.push_back({std::make_unique<RE2>(
                         R"(([\w\->\.:\[\]\(\)]+?)\s*\?->\s*([^;]+);)"),
                     R"(if(\1)
{
\1->\2;
})",
                     {"?->"}});
        // var = (cond) ? a : b;
        v.push_back(
            {std::make_unique<RE2>(
                 R"((?m)^(\s*)(?:[^=\n;]*?\b)?(\w+)\s*=\s*\(([^\)]+)\)\s*\?\s*([^:;]+)\s*:\s*([^;\n]+);)"),
             R"(\1\2 = None
\1if(\3)
{
\1\2 = \4;
}
\1else
{
\1\2 = \5;
})",
             {"?"}});
        // map_remove_if
        v.push_back(
            {std::make_unique<RE2>(
                 R"(map_remove_if\(([\w\d\-\>\.\[\]]+),\s*\((\w+),\s*(\w+)\s*:>\s*(.+)\)\))"),
             R"(\1 = [@[\2: \3 for \2, \3 in \1.items() if not(\4)]@])",
             {"map_remove_if"}});
        // list_remove_if
        v.push_back(
            {std::make_unique<RE2>(
                 R"(list_remove_if\(([\w\d\-\>\[\]]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
             R"__(
\1 = [\2 for \2 in \1 if not(\3)]
)__",
             {"list_remove_if"}});
        // map_do_if
        v.push_back(
            {std::make_unique<RE2>(
                 R"(map_do_if\(([\w\d\-\>\.\[\]]+),\s*\((\w+),\s*(\w+?)\s*:>\s*(.+?):>\s*(.+)\)\);)"),
             R"__(__keys__ = list(\1.keys())
for \2 in __keys__:
{ 
    \3 = \1.get(\2)
    if \4: 
    {
        \5
    }
}
)__",
             {"map_do_if"}});
        // list_do_if
        v.push_back(
            {std::make_unique<RE2>(
                 R"(list_do_if\(([\w\d\-\>\.\[\]]+),\s*\((\w+)\s*:>\s*(.+?):>\s*(.+)\)\);)"),
             R"__(
__size__ = len(\1)
__index__ = 0
while __index__ < __size__:
{
    \2 = \1[__index__]
    if \3:
    {
        \4
        if __size__ != len(\1):
        {
            __size__ = len(\1)
            __index__ -= 1
        }
    }
    __index__ += 1
}
)__",
             {"list_do_if"}});
        // list_do
        v.push_back(
            {std::make_unique<RE2>(
                 R"(list_do\(([\w\d\-\>\.\[\]]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
             R"__(
for __index__ in range(len(\1)):
{
    \2 = \1[__index__]
    \3
})__",
             {"list_do"}});
        // DataStorage shared getter
        v.push_back(
            {std::make_unique<RE2>(R"(\bDataStorage::shared\(\).get<(\w+)>)"),
             R"(DataStorage::shared().get\1)",
             {"DataStorage::shared"}});
        // Factory::X<Y> -> Factory.X
        v.push_back({std::make_unique<RE2>(R"(\bFactory::(.+)<\w+>)"),
                     R"(Factory.\1)",
                     {"Factory::"}});
        // range-for and while/if/else
        v.push_back({std::make_unique<RE2>(
                         R"(\bfor\s*\(\s*\w+[\s&\*]*(\w+)\s*:\s*(.+)\s*\))"),
                     R"(for \1 in \2:)",
                     {"for"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\bfor\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+<([\.\->\w]+?);\s*\+\+\w+\s*\))"),
             R"(for \1 in range(int(\2), int(\3)):)",
             {"for"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\bfor\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+>([\.\->\w]+?);\s*--\w+\s*\))"),
             R"(for \1 in range(int(\2), int(\3), -1):)",
             {"for"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\bfor\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+<([\.\->\w]+?);\s*\w+\+=(\w+)\s*\))"),
             R"(for \1 in range(int(\2), int(\3), int(\4)):)",
             {"for"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\bfor\s*\(\s*\w+\s*(\w+)=(\w+);\s*\w+>([\.\->\w]+?);\s*\w+-=(\w+)\s*\))"),
             R"(for \1 in range(int(\2), int(\3), -int(\4)):)",
             {"for"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\bfor\s*\(auto&&\s*\[(\w+),\s*(\w+)\]\s*:\s*(.+)\))"),
             R"(for \1, \2 in \3.items():)",
             {"for"}});
        v.push_back({std::make_unique<RE2>(R"(\bwhile\s*\((.+)\))"),
                     R"(while(\1):)",
                     {"while"}});
        v.push_back({std::make_unique<RE2>(R"(\belse\s+if\s*\(\s*(.+)\s*\))"),
                     R"(elif \1:)",
                     {"else"}});
        v.push_back({std::make_unique<RE2>(R"(\bif\s*\(\s*(.+)\s*\))"),
                     R"(if \1:)",
                     {"if"}});
        v.push_back({std::make_unique<RE2>(R"(\bif\s*!([^:]+):)"),
                     R"(if not \1:)",
                     {"if"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\belse\b)"), R"(else:)", {"else"}});
        v.push_back({std::make_unique<RE2>(R"(std::string (\w+);)"),
                     R"(\1 = "")",
                     {"std::string"}});
        v.push_back(
            {std::make_unique<RE2>(R"((\w+)\s+(\w+);)"), R"(\2 = \1())", {}});
        v.push_back({std::make_unique<RE2>(R"((\w+) = return\(\))"),
                     R"(return \1)",
                     {"return"}});
        v.push_back({std::make_unique<RE2>(R"(std::vector<.+>\s+(\w+))"),
                     R"(\1 = list())",
                     {"std::vector"}});
        v.push_back({std::make_unique<RE2>(R"(std::string\s+(\w+))"),
                     R"(\1)",
                     {"std::string"}});
        v.push_back({std::make_unique<RE2>(R"(\blist<.+>\s+(\w+))"),
                     R"(\1 = list())",
                     {"list<"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\bmap<([<:>\w\s\*&]+),\s*([<:>\w\s\*&]+)>\s*(\w+))"),
             R"(\3 = dict())",
             {"map<"}});
        // nullish coalescing operator: a ?? b
        v.push_back(
            {std::make_unique<RE2>(
                 R"([\w\*]+ ([\w\->\.]+)\s*=\s*([\w\->\.]+)\s*\?\?\s*([\w\->\.]+))"),
             R"(\1 = (\2) if (\2 is not None) else (\3))",
             {"??"}});
        // simple type decorations removal
        v.push_back(
            {std::make_unique<RE2>(R"(\bauto\&* (\w+))"), R"(\1)", {"auto"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bstring (\w+))"), R"(\1)", {"string"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bint (\w+))"), R"(\1)", {"int"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bfloat (\w+))"), R"(\1)", {"float"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bbool (\w+))"), R"(\1)", {"bool"}});
        v.push_back({std::make_unique<RE2>(R"(std::min<int>)"),
                     R"(std::min)",
                     {"std::min"}});
        v.push_back({std::make_unique<RE2>(R"(std::min<float>)"),
                     R"(std::min)",
                     {"std::min"}});
        v.push_back({std::make_unique<RE2>(R"(std::max<int>)"),
                     R"(std::max)",
                     {"std::max"}});
        v.push_back({std::make_unique<RE2>(R"(std::max<float>)"),
                     R"(std::max)",
                     {"std::max"}});
        v.push_back({std::make_unique<RE2>(R"(\b\w+ (\w+)\s+=\s+(.+);)"),
                     R"(\1 = \2)",
                     {"="}});
        v.push_back({std::make_unique<RE2>(R"((\w)->)"), R"(\1.)", {}});
        v.push_back({std::make_unique<RE2>(R"(\+\+(\w+))"), R"(\1 += 1)", {}});
        v.push_back({std::make_unique<RE2>(R"((\w+)\+\+)"), R"(\1 += 1)", {}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bdelete\b (\w*);)"), R"(pass)", {}});
        v.push_back({std::make_unique<RE2>(R"(&(\w+))"), R"(\1)", {}});
        v.push_back({std::make_unique<RE2>(R"(!(\w+))"), R"(not \1)", {}});
        v.push_back({std::make_unique<RE2>(R"(!\()"), R"(not ()", {}});
        v.push_back({std::make_unique<RE2>(R"__(make_intrusive<(\w+)>\(\))__"),
                     R"(make_intrusive(\1))",
                     {"make_intrusive"}});
        v.push_back({std::make_unique<RE2>(R"(\bnew\s*(\w+)\s*\(\s*?\))"),
                     R"(make_intrusive(\1))",
                     {"new"}});
        v.push_back({std::make_unique<RE2>(R"(\bnew\s*(\w+)\s*\((.*?)\))"),
                     R"(make_intrusive(\1, \2))",
                     {"new"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bassert\(.+\);)"), R"()", {"assert "}});
        v.push_back(
            {std::make_unique<RE2>(R"((\b[-0-9]+)\.f\b)"), R"(\1.0)", {}});
        v.push_back({std::make_unique<RE2>(R"((\b[-0-9]+)\.([-0-9]*)f\b)"),
                     R"(\1.\2)",
                     {}});
        v.push_back({std::make_unique<RE2>(R"(([*+-/\s])log\((.+?)\))"),
                     R"(\1math.log(\2))",
                     {"log"}});
        v.push_back({std::make_unique<RE2>(R"(\brandom_float\(\))"),
                     R"(random.random())",
                     {"random_float"}});
        v.push_back({std::make_unique<RE2>(R"(\brandom_int\((.+)?,\s*(.+)?\))"),
                     R"(random.randint(\1, \2 - 1))",
                     {"random_int"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bthis\b)"), R"(self)", {"this"}});
        v.push_back({std::make_unique<RE2>(R"(, std::placeholders::_\d)"),
                     R"()",
                     {"std::placeholders"}});
        v.push_back(
            {std::make_unique<RE2>(
                 R"(dynamic_pointer_cast_intrusive<(\w+),\s*(\w+)>\((.+?)\))"),
             R"((\3 if isinstance(\3, \1) else None))",
             {"dynamic_pointer_cast_intrusive"}});
        v.push_back({std::make_unique<RE2>(
                         R"(dynamic_pointer_cast_intrusive<(.+)>\((.+?)\))"),
                     R"((\2 if isinstance(\2, \1) else None))",
                     {"dynamic_pointer_cast_intrusive"}});
        v.push_back({std::make_unique<RE2>(R"(([\w\.]+?)\s*!=\s*False)"),
                     R"((\1))",
                     {"False"}});
        v.push_back({std::make_unique<RE2>(R"(([\w\.]+?)\s*==\s*False)"),
                     R"(not (\1))",
                     {"False"}});
        v.push_back({std::make_unique<RE2>(R"(\bstrTo<(\w+)>\((.+?)\))"),
                     R"(strTo(\2, \1))",
                     {"strTo"}});
        v.push_back({std::make_unique<RE2>(R"(\btoStr\((.+?)\))"),
                     R"(str(\1))",
                     {"toStr"}});
        // template arguments
        v.push_back(
            {std::make_unique<RE2>(R"((\w+)<(.+?)>\(\))"), R"(\1(\2))", {"<"}});
        v.push_back({std::make_unique<RE2>(R"((\w+)<(.+?)>\((.+?)\))"),
                     R"(\1(\3, \2))",
                     {"<"}});
        // strcat and .at()
        v.push_back({std::make_unique<RE2>(R"(std::strcat\((.+?),\s*(.+?)\))"),
                     R"(((\1)+(\2)))",
                     {"std::strcat"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\.at\((.*?)\))"), R"([\1])", {"at"}});
        // Exception try/catch
        v.push_back(
            {std::make_unique<RE2>(
                 R"(\btry\n\s*{([\s\S.]+?)}\n\s*catch\(((\w+)\s*(\w*))\)\n\s+{([\s\S.]+?)})"),
             R"__(try:
{
\1
}
except BaseException as \4:
{
\5
}
)__",
             {"try"}});
        v.push_back({std::make_unique<RE2>(R"(\bthrow Exception\((.*?)\))"),
                     R"(raise BaseException())",
                     {"throw"}});
        v.push_back(
            {std::make_unique<RE2>(R"(\bthrow make_intrusive\(Exception\))"),
             R"(raise BaseException())",
             {"throw"}});
        v.push_back({std::make_unique<RE2>(R"(mg_swap\((.+)\s*,\s*(.+)\))"),
                     R"(\1, \2 = \2, \1)",
                     {"mg_swap"}});
        return v;
    }();

    // PEP8-related regex tweaks
    static inline std::vector<RegexPattern> pep8 = [] {
        std::vector<RegexPattern> v;
        v.push_back({std::make_unique<RE2>(R"(([\w\.]+?)\s*!=\s*False)"),
                     R"((\1))",
                     {}});
        v.push_back({std::make_unique<RE2>(R"(([\w\.]+?)\s*==\s*False)"),
                     R"(not (\1))",
                     {}});
        v.push_back({std::make_unique<RE2>(R"(([\w\.]+?)\s*!=\s*True)"),
                     R"(not (\1))",
                     {}});
        v.push_back({std::make_unique<RE2>(R"(([\w\.]+?)\s*==\s*True)"),
                     R"((\1))",
                     {}});
        return v;
    }();

    // Simple text replacements
    static inline std::vector<std::pair<std::string, std::string>> replaces = {
        {"this.", "self."},
        {"->", "."},
        {"::", "."},
        {"&&", " and "},
        {"||", " or "},
        {"  and  ", " and "},
        {"  or  ", " or "},
        {"true", "True"},
        {"false", "False"},
        {"nullptr", "None"},
        {"std.round", "int"},
        {"std.fabs", "abs"},
        {"std.ceil", "math.ceil"},
        {"std.floor", "math.floor"},
        {"std.sqrt", "math.sqrt"},
        {"std.min", "min"},
        {"std.max", "max"},
        {"!= None", "is not None"},
        {"== None", "is None"},
        {";", ""},
    };
};
