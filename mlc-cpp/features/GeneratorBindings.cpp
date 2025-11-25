//
//  GeneratorBindings.cpp
//  mlc-cpp
//

#include "GeneratorBindings.hpp"

#include "../module_cpp/WriterCpp.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Model.hpp"
#include "../utils/Common.hpp"
#include "../utils/Error.hpp"
#include <algorithm>
#include <cctype>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

const std::unordered_map<std::string, std::string> kTypeAliases = {
    {"string", "std::string"},     {"std::string", "std::string"},
    {"bool", "bool"},              {"float", "float"},
    {"double", "double"},          {"int", "int"},
    {"int32_t", "int32_t"},        {"int64_t", "int64_t"},
    {"uint", "unsigned int"},      {"unsigned", "unsigned int"},
    {"uint32_t", "uint32_t"},      {"uint64_t", "uint64_t"}};

std::string canonicalType(const std::string &raw) {
    auto it = kTypeAliases.find(raw);
    return it == kTypeAliases.end() ? raw : it->second;
}

bool isSupportedType(const std::string &raw) {
    return kTypeAliases.find(raw) != kTypeAliases.end();
}

bool getArgumentType(const Object &arg, std::string &cppType) {
    if (arg.is_pointer)
        return false;
    if (!isSupportedType(arg.type))
        return false;
    cppType = canonicalType(arg.type);
    return true;
}

bool getReturnType(const Function &fn, std::string &cppType) {
    if (fn.return_type.type == "void")
        return false;
    if (!isSupportedType(fn.return_type.type))
        return false;
    cppType = canonicalType(fn.return_type.type);
    return true;
}

std::string describe(const Class &cls, const Function &fn) {
    return cls.name + "::" + fn.name;
}

std::string includeForClass(const Class &cls) {
    if (cls.group.empty())
        return cls.name + ".h";
    return cls.group + "/" + cls.name + ".h";
}

std::string defaultValueExpr(const Object &arg, const std::string &cppType) {
    if (!arg.value.empty()) {
        if (cppType == "std::string")
            return "std::string(" + arg.value + ")";
        return arg.value;
    }
    if (cppType == "std::string")
        return "std::string()";
    if (cppType == "float")
        return "0.0f";
    if (cppType == "double")
        return "0.0";
    if (cppType == "bool")
        return "false";
    return "0";
}

std::string getterName(const std::shared_ptr<Class> &cls, const Function &fn) {
    return cls->name + "::" + fn.name;
}

struct ArgumentInfo {
    std::string original;
    std::string variable;
    std::string cppType;
    std::string defaultExpr;
    bool hasDefault;
};

std::vector<ArgumentInfo> buildArgumentInfo(const Function &fn) {
    std::vector<ArgumentInfo> result;
    std::unordered_set<std::string> usedNames;
    result.reserve(fn.callable_args.size());

    for (size_t idx = 0; idx < fn.callable_args.size(); ++idx) {
        const auto &arg = fn.callable_args[idx];
        std::string cppType = canonicalType(arg.type);
        std::string varName =
            arg.name.empty() ? ("arg" + std::to_string(idx)) : arg.name;
        varName = GeneratorBindings::sanitize(varName);
        if (varName.empty())
            varName = "arg" + std::to_string(idx);
        if (std::isdigit(static_cast<unsigned char>(varName[0])))
            varName.insert(varName.begin(), '_');
        std::string unique = varName;
        size_t suffix = 1;
        while (usedNames.count(unique))
            unique = varName + "_" + std::to_string(suffix++);
        usedNames.insert(unique);

        result.push_back({arg.name,
                          unique,
                          cppType,
                          defaultValueExpr(arg, cppType),
                          !arg.value.empty()});
    }

    return result;
}

struct MethodRenderEntry {
    std::shared_ptr<Class> cls;
    const Function *fn;
    std::string getter;
    std::string callName;
    std::vector<ArgumentInfo> args;
};

} // namespace

GeneratorBindings::GeneratorBindings(const FeatureBindings &config)
    : _config(config) {}

void GeneratorBindings::generate(Model &model) {
    if(model.config.language != "cpp"){
        return;
    }
    
    auto methodEntries = collectBindings(model);
    auto observableEntries = collectObservableBindings(model);
    if (methodEntries.empty() && observableEntries.empty())
        return;

    model.addFile(nullptr, "Bindings_.h",
                  renderBindingClassHeader(model, methodEntries));
    model.addFile(nullptr, "Bindings.h",
                  renderBindingFuncHeader(model, methodEntries));
    model.addFile(nullptr, "Bindings.cpp",
                  renderBindingFuncSource(model, methodEntries,
                                          observableEntries));
}

void GeneratorBindings::modifySources(Model &, const std::shared_ptr<Class> &,
                                      std::string &, std::string &) {}

std::vector<GeneratorBindings::BindingEntry>
GeneratorBindings::collectBindings(Model &model) const {
    std::vector<BindingEntry> bindings;
    bindings.reserve(model.classes.size());

    for (auto &cls : model.classes) {
        if (!cls || !cls->has_bindings)
            continue;
        if (model.is_skip(*cls))
            continue;

        for (auto &fn : cls->functions) {
            if (!fn.is_binding)
                continue;
            if (model.is_skip(fn))
                continue;
            if (!isSupported(*cls, fn))
                continue;
            bindings.push_back({cls, &fn});
        }
    }

    std::sort(bindings.begin(), bindings.end(),
              [](const BindingEntry &lhs, const BindingEntry &rhs) {
                  auto l = getterName(lhs.cls, *lhs.fn);
                  auto r = getterName(rhs.cls, *rhs.fn);
                  if (l == r)
                      return lhs.cls->group < rhs.cls->group;
                  return l < r;
              });
    bindings.erase(std::unique(bindings.begin(), bindings.end(),
                               [](const BindingEntry &a,
                                  const BindingEntry &b) {
                                   return a.cls == b.cls && a.fn == b.fn;
                               }),
                   bindings.end());
    return bindings;
}

std::vector<GeneratorBindings::ObservableEntry>
GeneratorBindings::collectObservableBindings(Model &model) const {
    std::vector<ObservableEntry> result;
    result.reserve(model.classes.size());
    for (auto &cls : model.classes) {
        if (!cls || !cls->has_bindings)
            continue;
        if (model.is_skip(*cls))
            continue;
        for (auto &member : cls->members) {
            if (!member.is_binding)
                continue;
            if (member.type != "Observable")
                continue;
            result.push_back({cls, &member});
        }
    }
    return result;
}

bool GeneratorBindings::isSupported(const Class &cls,
                                    const Function &fn) const {
    const std::string name = describe(cls, fn);
    if (fn.is_static) {
        Log::warning("Skip binding for " + name +
                     ": static methods are not supported");
        return false;
    }
    if (!fn.template_args.empty()) {
        Log::warning("Skip binding for " + name +
                     ": templated methods are not supported");
        return false;
    }
    if (fn.access != AccessSpecifier::m_public) {
        Log::warning("Skip binding for " + name +
                     ": only public methods are supported");
        return false;
    }
    if (fn.name.empty()) {
        Log::warning("Skip binding for unnamed method in " + cls.name);
        return false;
    }

    std::string retType;
    if (!getReturnType(fn, retType)) {
        Log::warning("Skip binding for " + name +
                     ": unsupported return type '" + fn.return_type.type + "'");
        return false;
    }

    for (size_t idx = 0; idx < fn.callable_args.size(); ++idx) {
        const auto &arg = fn.callable_args[idx];
        if (arg.name.empty()) {
            Log::warning("Skip binding for " + name +
                         ": argument #" + std::to_string(idx) +
                         " must have a name");
            return false;
        }
        std::string cppType;
        if (!getArgumentType(arg, cppType)) {
            Log::warning("Skip binding for " + name +
                         ": unsupported argument type '" + arg.type +
                         "' for parameter '" + arg.name + "'");
            return false;
        }
    }
    return true;
}

std::string GeneratorBindings::renderBindingClassHeader(
    const Model &model, const std::vector<BindingEntry> &entries) const {
    const std::string ns =
        model.config.namespace_name.empty() ? "mg" : model.config.namespace_name;

    std::set<std::string> stdIncludes = {
        "<cstdint>", "<map>", "<string>", "<type_traits>", "<unordered_map>",
        "<utility>"};
    std::set<std::string> classIncludes;
    std::vector<MethodRenderEntry> renderEntries;
    renderEntries.reserve(entries.size());

    for (const auto &entry : entries) {
        MethodRenderEntry info;
        info.cls = entry.cls;
        info.fn = entry.fn;
        info.getter = getterName(entry.cls, *entry.fn);
        info.callName = GeneratorBindings::sanitize(info.getter);
        info.args = buildArgumentInfo(*entry.fn);
        classIncludes.insert(includeForClass(*entry.cls));
        renderEntries.push_back(std::move(info));
    }

    std::ostringstream out;
    out << "#pragma once\n\n";
    for (const auto &inc : stdIncludes)
        out << "#include " << inc << "\n";
    out << "\n#include \"" << ns << "_extensions.h\"\n";
    for (const auto &inc : classIncludes)
        out << "#include \"" << inc << "\"\n";
    out << "\nnamespace " << ns << " {\n\n";
    out << "class Bindings {\npublic:\n";
    out << "    using Arguments = std::map<std::string, std::string>;\n\n";
    out << "    class Value {\n    public:\n";
    out << "        Value() = default;\n";
    out << "        explicit Value(std::string raw);\n";
    out << "        const std::string &raw() const { return raw_; }\n";
    out << "        bool empty() const { return raw_.empty(); }\n";
    out << "        template <class TValue> TValue as() const;\n";
    out << "        operator float() const;\n";
    out << "        operator std::string() const;\n";
    out << "        template <class TValue> static Value from(const TValue &value);\n";
    out << "        static Value from(const std::string &value);\n";
    out << "        static Value from(const char *value);\n";
    out << "\n    private:\n";
    out << "        std::string raw_;\n";
    out << "    };\n\n";
    out << "    template <class TObject>\n";
    out << "    static Value get(TObject *object, const std::string &getter, const Arguments &arguments, Value default_value = "
           "Value());\n";
    out << "    template <class TObject>\n";
    out << "    static Value get(const TObject *object, const std::string &getter, const Arguments &arguments, Value default_value = "
           "Value());\n";
    out << "    template <class TObject>\n";
    out << "    static Value get(TObject *object, const std::string &getter, const std::string &arguments, Value default_value = "
           "Value());\n";
    out << "    template <class TObject>\n";
    out << "    static Value get(const TObject *object, const std::string &getter, const std::string &arguments, Value default_value = "
           "Value());\n";
    out << "    template <class TObject>\n";
    out << "    static bool try_get(TObject *object, const std::string &getter, const Arguments &arguments, Value &out_value);\n";
    out << "    template <class TObject>\n";
    out << "    static bool try_get(const TObject *object, const std::string &getter, const Arguments &arguments, Value &out_value);\n";
    out << "    template <class TObject>\n";
    out << "    static bool try_get(TObject *object, const std::string &getter, const std::string &arguments, Value &out_value);\n";
    out << "    template <class TObject>\n";
    out << "    static bool try_get(const TObject *object, const std::string &getter, const std::string &arguments, Value &out_value);\n\n";
    out << "    static Arguments parse(const std::string &text);\n";
        out << "private:\n";
    out << "    static std::string trim(const std::string &value);\n";
    out << "    static bool has_argument(const Arguments &arguments, const "
           "std::string &name);\n";
    out << "    template <class TValue>\n";
    out << "    static TValue read_argument(const Arguments &arguments, const std::string &name, const TValue &default_value);\n";
    out << "    static std::string read_argument(const Arguments &arguments, const std::string &name, const std::string &default_value);\n";
    out << "    template <class TObject>\n";
    out << "    static bool dispatch(TObject *object, const std::string &getter, const Arguments &arguments, Value &out_value);\n";
    for (const auto &entry : renderEntries) {
        out << "    static Value " << entry.callName << "("
            << entry.cls->name
            << " *object, const Arguments &arguments);\n";
    }
    out << "};\n\n";

    out << "inline Bindings::Value::Value(std::string raw)\n";
    out << "    : raw_(std::move(raw)) {}\n\n";
    out << "template <class TValue>\n";
    out << "inline TValue Bindings::Value::as() const {\n";
    out << "    return strTo<TValue>(raw_);\n";
    out << "}\n\n";
    out << "inline Bindings::Value::operator float() const {\n";
    out << "    return strTo<float>(raw_);\n";
    out << "}\n\n";
    out << "inline Bindings::Value::operator std::string() const {\n";
    out << "    return raw_;\n";
    out << "}\n\n";
    out << "template <class TValue>\n";
    out << "inline Bindings::Value Bindings::Value::from(const TValue &value) {\n";
    out << "    return Value(toStr(value));\n";
    out << "}\n\n";
    out << "inline Bindings::Value Bindings::Value::from(const std::string &value) "
           "{\n";
    out << "    return Value(value);\n";
    out << "}\n\n";
    out << "inline Bindings::Value Bindings::Value::from(const char *value) {\n";
    out << "    return Value(value ? value : \"\");\n";
    out << "}\n\n";

    out << "inline Bindings::Arguments Bindings::parse(const std::string &text) "
           "{\n";
    out << "    Arguments args;\n";
    out << "    size_t pos = 0;\n";
    out << "    auto is_delim = [](char c) {\n";
    out << "        return c == ' ' || c == '\\t' || c == '\\n' || c == '\\r' || "
           "c == ';' || c == ',';\n";
    out << "    };\n";
    out << "    while (pos < text.size()) {\n";
    out << "        while (pos < text.size() && is_delim(text[pos]))\n";
    out << "            ++pos;\n";
    out << "        if (pos >= text.size())\n";
    out << "            break;\n";
    out << "        size_t key_start = pos;\n";
    out << "        while (pos < text.size() && text[pos] != '=')\n";
    out << "            ++pos;\n";
    out << "        if (pos >= text.size())\n";
    out << "            break;\n";
    out << "        std::string key = trim(text.substr(key_start, pos - key_start));\n";
    out << "        ++pos;\n";
    out << "        if (key.empty())\n";
    out << "            continue;\n";
    out << "        std::string value;\n";
    out << "        if (pos < text.size() && (text[pos] == '\"' || text[pos] == "
           "'\\'')) {\n";
    out << "            char quote = text[pos++];\n";
    out << "            size_t value_start = pos;\n";
    out << "            while (pos < text.size() && text[pos] != quote)\n";
    out << "                ++pos;\n";
    out << "            value = text.substr(value_start, pos - value_start);\n";
    out << "            if (pos < text.size())\n";
    out << "                ++pos;\n";
    out << "        } else {\n";
    out << "            size_t value_start = pos;\n";
    out << "            while (pos < text.size() && !is_delim(text[pos]))\n";
    out << "                ++pos;\n";
    out << "            value = text.substr(value_start, pos - value_start);\n";
    out << "        }\n";
    out << "        args[key] = value;\n";
    out << "    }\n";
    out << "    return args;\n";
    out << "}\n\n";

    out << "inline std::string Bindings::trim(const std::string &value) {\n";
    out << "    size_t start = 0;\n";
    out << "    while (start < value.size() &&\n";
    out << "           (value[start] == ' ' || value[start] == '\\t' || value[start] "
           "== '\\n' || value[start] == '\\r'))\n";
    out << "        ++start;\n";
    out << "    size_t end = value.size();\n";
    out << "    while (end > start &&\n";
    out << "           (value[end - 1] == ' ' || value[end - 1] == '\\t' || "
           "value[end - 1] == '\\n' || value[end - 1] == '\\r'))\n";
    out << "        --end;\n";
    out << "    return value.substr(start, end - start);\n";
    out << "}\n\n";

    out << "inline bool Bindings::has_argument(const Arguments &arguments,\n";
    out << "                                    const std::string &name) {\n";
    out << "    return arguments.find(name) != arguments.end();\n";
    out << "}\n\n";

    out << "template <class TValue>\n";
    out << "inline TValue Bindings::read_argument(const Arguments &arguments,\n";
    out << "                                       const std::string &name,\n";
    out << "                                       const TValue &default_value) {\n";
    out << "    auto it = arguments.find(name);\n";
    out << "    if (it == arguments.end())\n";
    out << "        return default_value;\n";
    out << "    return strTo<TValue>(it->second);\n";
    out << "}\n\n";

    out << "inline std::string Bindings::read_argument(\n";
    out << "    const Arguments &arguments, const std::string &name,\n";
    out << "    const std::string &default_value) {\n";
    out << "    auto it = arguments.find(name);\n";
    out << "    if (it == arguments.end())\n";
    out << "        return default_value;\n";
    out << "    return it->second;\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline Bindings::Value Bindings::get(TObject *object, const std::string &getter, const Arguments &arguments, Value default_value) {\n";
    out << "    Value result;\n";
    out << "    if (try_get(object, getter, arguments, result))\n";
    out << "        return result;\n";
    out << "    return default_value;\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline Bindings::Value Bindings::get(const TObject *object, const std::string &getter, const Arguments &arguments, Value default_value) {\n";
    out << "    return get(const_cast<TObject *>(object), getter, arguments, std::move(default_value));\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline Bindings::Value Bindings::get(TObject *object, const std::string &getter, const std::string &arguments, Value default_value) {\n";
    out << "    return get(object, getter, parse(arguments), std::move(default_value));\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline Bindings::Value Bindings::get(const TObject *object, const std::string &getter, const std::string &arguments, Value default_value) {\n";
    out << "    return get(const_cast<TObject *>(object), getter, arguments, std::move(default_value));\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline bool Bindings::try_get(TObject *object, const std::string &getter, const Arguments &arguments, Value &out_value) {\n";
    out << "    return dispatch(object, getter, arguments, out_value);\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline bool Bindings::try_get(const TObject *object, const std::string &getter, const Arguments &arguments, Value &out_value) {\n";
    out << "    return try_get(const_cast<TObject *>(object), getter, arguments, out_value);\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline bool Bindings::try_get(TObject *object, const std::string &getter, const std::string &arguments, Value &out_value) {\n";
    out << "    return try_get(object, getter, parse(arguments), out_value);\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline bool Bindings::try_get(const TObject *object, const std::string &getter, const std::string &arguments, Value &out_value) {\n";
    out << "    return try_get(const_cast<TObject *>(object), getter, arguments, out_value);\n";
    out << "}\n\n";

    out << "template <class TObject>\n";
    out << "inline bool Bindings::dispatch(TObject *object, const std::string &getter, const Arguments &arguments, Value &out_value) {\n";
    out << "    if (!object)\n";
    out << "        return false;\n";
    out << "    static_assert(std::is_polymorphic_v<TObject>, \"Bindings::get expects a polymorphic type\");\n";
    out << "    if (getter.empty())\n";
    out << "        return false;\n";
    out << "    using Dispatcher = bool (*)(TObject *, const Arguments &, Value &);\n";
    out << "    static const std::unordered_map<std::string, Dispatcher> dispatchers = "
           "{\n";
    for (const auto &entry : renderEntries) {
        out << "        {\"" << entry.getter
            << "\", [](TObject *object, const Arguments &arguments, Value &out_value) -> bool {\n";
        out << "             auto typed = dynamic_cast<" << entry.cls->name
            << " *>(object);\n";
        out << "             if (!typed)\n";
        out << "                 return false;\n";
        out << "             out_value = " << entry.callName
            << "(typed, arguments);\n";
        out << "             return true;\n";
        out << "         }},\n";
    }
    out << "    };\n";
    out << "    auto it = dispatchers.find(getter);\n";
    out << "    if (it == dispatchers.end())\n";
    out << "        return false;\n";
    out << "    return it->second(object, arguments, out_value);\n";
    out << "}\n\n";

    for (const auto &entry : renderEntries) {
        out << "inline Bindings::Value Bindings::" << entry.callName << "("
            << entry.cls->name
            << " *object, const Arguments &arguments) {\n";
        out << "    if (!object)\n";
        out << "        return Value();\n";
        for (const auto &arg : entry.args) {
            out << "    const bool has_" << arg.variable
                << " = has_argument(arguments, \"" << arg.original << "\");\n";
            if (!arg.hasDefault) {
                out << "    if (!has_" << arg.variable << ")\n";
                out << "        return Value();\n";
            }
            out << "    " << arg.cppType << " " << arg.variable << " = has_"
                << arg.variable << " ? read_argument<" << arg.cppType
                << ">(arguments, \"" << arg.original << "\", " << arg.defaultExpr
                << ") : " << arg.defaultExpr << ";\n";
        }
        out << "    return Value::from(object->" << entry.fn->name << "(";
        for (size_t i = 0; i < entry.args.size(); ++i) {
            if (i)
                out << ", ";
            out << entry.args[i].variable;
        }
        out << "));\n";
        out << "}\n\n";
    }

    out << "} // namespace " << ns << "\n";
    return out.str();
}

std::string GeneratorBindings::renderBindingFuncHeader(
    const Model &, const std::vector<BindingEntry> &) const {
    static const std::string BINDINGS_HPP = R"(#ifndef mg_Bindings_h
#define mg_Bindings_h
#include <string>
#include "Observable.h"
namespace mg
{
std::string get_binding_value(const std::string& description, const std::string& args);
ObservableBase* get_binding_observable_raw(const std::string& description);
Observable<void()>* get_binding_observable(const std::string& description);
template <class TObservable>
inline TObservable* get_binding_observable_as(const std::string& description)
{
    return dynamic_cast<TObservable*>(get_binding_observable_raw(description));
}
}
#endif
)";
    return BINDINGS_HPP;
}

std::string GeneratorBindings::renderBindingFuncSource(
    const Model &model, const std::vector<BindingEntry> &entries,
    const std::vector<ObservableEntry> &observables) const {
    std::string source = R"__(
#include <cassert>
#include <string>
#include <unordered_map>
#include "Observable.h"
#include "Bindings_.h"
{includes}

namespace mg
{
template<class TRes, class TObj>
TRes get_binding_value(const TObj* object, const std::string& description, const Bindings::Arguments& args)
{
    static auto default_value = Bindings::Value::from(0);
    auto binding = Bindings::get<TObj>(object, description, args, default_value);
    return binding.template as<TRes>();
}

std::string get_binding_value(const std::string& description, const std::string& args)
{
    auto t = description.find("::");
    assert(t != std::string::npos);
    auto class_name = description.substr(0, t);

    auto arguments = Bindings::parse(args);

    using Dispatcher = std::string (*)(const std::string&, const Bindings::Arguments&);
    static const std::unordered_map<std::string, Dispatcher> dispatchers = {
{value_dispatchers}
    };
    auto it = dispatchers.find(class_name);
    if (it == dispatchers.end())
    {
        assert(0);
        return "";
    }
    return it->second(description, arguments);
}

ObservableBase* get_binding_observable_raw(const std::string& description)
{
    using ObservableDispatcher = ObservableBase* (*)();
    static const std::unordered_map<std::string, ObservableDispatcher> observables = {
{observable_dispatchers}
    };
    auto it = observables.find(description);
    if (it == observables.end())
    {
        assert(0);
        return nullptr;
    }
    return it->second();
}

Observable<void()>* get_binding_observable(const std::string& description)
{
    return dynamic_cast<Observable<void()>*>(get_binding_observable_raw(description));
}
}
)__";

    const std::string storage_getter = R"(
        {"{0}", [](const std::string& description, const Bindings::Arguments& arguments) -> std::string {
            auto it = arguments.find("name");
            if (it == arguments.end())
                return std::string();
            auto obj = DataStorage::shared().get<{0}>(it->second);
            auto result = get_binding_value<std::string, {0}>(obj, description, arguments);
            return result;
        }},
)";

    const std::string getter_template = R"(
        {"{0}", [](const std::string& description, const Bindings::Arguments& arguments) -> std::string {
            auto obj = {1};
            auto result = get_binding_value<std::string, {0}>(obj, description, arguments);
            return result;
        }},
)";

    const std::string observable_template = R"(
        {"{0}::{1}", []() -> ObservableBase* {
            auto obj = {2};
            if (!obj)
                return nullptr;
            return &(obj->{1});
        }},
)";

    std::string valueDispatchers;
    bool needsDataStorage = false;
    std::set<std::string> processedClasses;
    for (auto &entry : entries) {
        if(model.is_skip(*entry.cls))
            continue;
        if (processedClasses.count(entry.cls->name))
            continue;
        if (entry.cls->is_storage) {
            valueDispatchers += format_indexes(storage_getter, entry.cls->name);
            processedClasses.insert(entry.cls->name);
            needsDataStorage = true;
            continue;
        }
        auto getter = _config.get_getter(entry.cls->name);
        if (getter.empty()) {
            Log::warning("Skip getter class for " + entry.cls->name);
            continue;
        }
        replace_all(getter, "{class_name}", entry.cls->name);
        valueDispatchers += format_indexes(getter_template, entry.cls->name, getter);
        processedClasses.insert(entry.cls->name);
    }

    std::string observableDispatchers;
    for (auto &entry : observables) {
        if(model.is_skip(*entry.member))
            continue;

        if (entry.cls->is_storage) {
            Log::warning("Skip observable binding for storage class " +
                         entry.cls->name + "::" + entry.member->name);
            continue;
        }
        auto getter = _config.get_getter(entry.cls->name);
        if (getter.empty()) {
            Log::warning("Skip getter class for observable " + entry.cls->name +
                         "::" + entry.member->name);
            continue;
        }
        replace_all(getter, "{class_name}", entry.cls->name);
        observableDispatchers += format_indexes(
            observable_template, entry.cls->name, entry.member->name, getter);
    }

    auto includes = WriterCpp::getIncludesForMethod(
        model, nullptr, valueDispatchers + observableDispatchers, {});
    std::string sincludes;
    for (auto &name : includes) {
        auto cls = model.get_class(name);
        if (!cls)
            continue;
        sincludes += "#include \"";
        if (!cls->group.empty())
            sincludes += cls->group + "/";
        sincludes += cls->name + ".h\"\n";
    }

    if (needsDataStorage)
        sincludes = "#include \"DataStorage.h\"\n" + sincludes;
    replace_all(source, "{value_dispatchers}", valueDispatchers);
    replace_all(source, "{observable_dispatchers}", observableDispatchers);
    replace_all(source, "{includes}", sincludes);
    return source;
}

std::string GeneratorBindings::sanitize(const std::string &value) {
    std::string out;
    out.reserve(value.size());
    for (char c : value) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_')
            out.push_back(c);
        else
            out.push_back('_');
    }
    if (!out.empty() && std::isdigit(static_cast<unsigned char>(out.front())))
        out.insert(out.begin(), '_');
    return out.empty() ? std::string("_binding") : out;
}
