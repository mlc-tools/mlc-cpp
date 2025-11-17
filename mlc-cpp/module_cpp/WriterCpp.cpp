//
//  WriterCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

// WriterCpp.cpp
#include "WriterCpp.hpp"
#include "../features/FeatureGenerator.hpp"
#include "Class.hpp"
#include "Common.hpp"
#include "Error.hpp"
#include "Function.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "RegexPatternCpp.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <unordered_set>

WriterCpp::WriterCpp() {}

std::vector<std::pair<std::string, std::string>>
WriterCpp::writeClass(const std::shared_ptr<Class> &cls) {
    objectsCache_.clear();
    methodsCache_.clear();
    methodsCacheWithTemplates_.clear();

    if (cls->name == "UnitType")
        std::cout << "";

    // Cache members
    for (auto &m : cls->members) {
        if (_model->is_skip(m))
            continue;
        auto [decl, init, stat] = writeObject(m);
        objectsCache_[m.name] = {decl, init, stat};
    }

    // Cache methods
    for (auto &fn : cls->functions) {
        if (_model->is_skip(fn))
            continue;
        auto [hpp, cpp] = writeFunction(fn);
        methodsCache_[&fn] = {hpp, cpp};
        if (!fn.template_args.empty())
            methodsCacheWithTemplates_.push_back(hpp);
    }

    // Generate header and source
    auto [header, incs, fwd, fwdOut] = writeHpp(cls);
    auto source = writeCpp(cls, incs, fwd, fwdOut);

    // Feature generators hook
    for (auto &gen : _model->feature_generators)
        gen->modifySources(*_model, cls, header, source);

    // Return files
    return {{getFilename(cls, "h"), prepareFileCodeStyleCpp(header)},
            {getFilename(cls, "cpp"), prepareFileCodeStyleCpp(source)}};
}

auto WriterCpp::writeObject(const Object &obj)
    -> std::tuple<std::string, std::string, std::string> {
    if (currentClass_->name == "TestStaticMembers") {
        std::cout << "";
    }
    if (obj.type == "DataUnit")
        std::cout << "";
    std::string decl, init, stat;
    if (currentClass_->type == "class") {
        decl = writeMemberDeclaration(obj);
        if (obj.is_static)
            stat = writeMemberStaticInit(*currentClass_, obj);
        else
            init = writeMemberInitialization(obj);
    } else { // enum
        decl = writeMemberEnumDeclaration(obj);
    }
    return {decl, init, stat};
}

std::pair<std::string, std::string>
WriterCpp::writeFunction(const Function &fn) {
    return {writeFunctionHpp(fn), writeFunctionCpp(fn)};
}

auto WriterCpp::writeHpp(const std::shared_ptr<Class> &cls)
    -> std::tuple<std::string, std::set<std::string>, std::set<std::string>,
                  std::set<std::string>> {
    if (cls->name == "BulletType") {
        std::cout << "";
    }
    const std::string ns = "mg";
    const std::string cn = cls->name;
    auto [incs, fwd, fwdOut] = getIncludesForHeader(cls);

    // Build functions section
    std::string funcs;
    if (cls->type == "enum") {
        funcs += cn + "(const BaseEnum& rhs):BaseEnum(rhs){}\n";
        funcs += "const " + cn +
                 "& operator =(const BaseEnum& rhs) { this->value = "
                 "rhs.operator int(); return *this; }\n";
    }
    AccessSpecifier lastAcc = AccessSpecifier::m_public;
    for (auto &fn : cls->functions) {
        if (fn.name == "constructor")
            continue;
        if (fn.access != lastAcc) {
            funcs += AccessSpecifierToString(fn.access) + ":\n";
            lastAcc = fn.access;
        }
        funcs += methodsCache_[&fn].first;
    }
    for (auto &templHpp : methodsCacheWithTemplates_) {
        auto more = getIncludesForMethod(*_model, cls, templHpp, incs);
        incs.insert(more.begin(), more.end());
    }

    // Build members section
    std::string membs;
    lastAcc = AccessSpecifier::m_public;
    for (auto &m : cls->members) {
        if (_model->is_skip(m))
            continue;
        if (m.access != lastAcc) {
            membs += AccessSpecifierToString(m.access) + ":\n";
            lastAcc = m.access;
        }
        membs += objectsCache_[m.name][0] + "\n";
    }

    // Destructor
    std::string dtor;
    if (cls->type != "enum") {
        dtor = (cls->has_virtual() ? "virtual " : "") + std::string("~") + cn +
               "();";
    }

    // Superclass
    std::string super;
    if (!cls->parent_class_name.empty())
        super = " : public " + cls->parent_class_name;

    // Constructor args
    std::string ctorArgs;
    if (!cls->constructors.empty())
        ctorArgs = createFunctionHppArgs(cls->constructors.at(0));

    // Format header
    std::string header = HEADER;
    // simple replace placeholders:
    auto replace = [&](const std::string &key, const std::string &val) {
        size_t p = 0;
        while ((p = header.find("{" + key + "}", p)) != std::string::npos) {
            header.replace(p, key.size() + 2, val);
            p += val.size();
        }
    };
    replace("namespace", ns);
    replace("class_name", cn);
    replace("includes", buildIncludes(cls, incs));
    replace("forward_declarations", buildForwardDeclarations(fwd));
    replace("forward_declarations_out", buildForwardDeclarations(fwdOut));
    replace("functions", funcs);
    replace("members", membs);
    replace("destructor", dtor);
    replace("superclass", super);
    replace("constructor_args", ctorArgs);

    return {header, incs, fwd, fwdOut};
}

std::string WriterCpp::writeCpp(const std::shared_ptr<Class> &cls,
                                const std::set<std::string> &incs,
                                const std::set<std::string> &fwd,
                                const std::set<std::string> &fwdOut) {
    if (cls->name == "TestStaticMembers") {
        std::cout << "";
    }
    const std::string ns = "mg";
    const std::string cn = cls->name;
    // Methods
    std::string funcs;
    for (auto &fn : cls->functions) {
        if (fn.name == "constructor")
            continue;
        funcs += methodsCache_[&fn].second;
    }
    // Inits
    std::string inits, statics;
    for (auto &m : cls->members) {
        if (_model->is_skip(m))
            continue;

        auto &arr = objectsCache_[m.name];
        if (!arr[1].empty()) {
            if (!inits.empty())
                inits += ", ";
            else
                inits += ": ";
            inits += arr[1] + "\n";
        }
        if (!arr[2].empty())
            statics += arr[2] + "\n";
    }
    // Destructor
    std::string dtor;
    if (cls->type != "enum") {
        dtor = cn + "::~" + cn + "()\n{\n}\n";
    }
    // Registration
    std::string reg;
    bool abstract = cls->is_abstract;
    if (!abstract) {
        for (auto &fn : cls->functions) {
            if (fn.is_abstract) {
                abstract = true;
                break;
            }
        }
    }
    if (_model->config.auto_registration && !abstract &&
        cls->has_method("get_type"))
        reg = "REGISTRATION_OBJECT(" + cn + ");\n";

    // Includes
    auto allInc = getIncludesForSource(cls, funcs, incs, fwd, fwdOut);

    // Constructor
    std::string ctorArgs, ctorBody;
    if (!cls->constructors.empty()) {
        ctorArgs = createFunctionCppArgs(cls->constructors.at(0));
        ctorBody = strip(cls->constructors.at(0).body);
    }

    // Format source
    std::string source = SOURCE;
    replace_all(source, "{namespace}", ns);
    replace_all(source, "{path_to_root}", getPathToRoot(cls));
    replace_all(source, "{class_name}", cn);
    replace_all(source, "{includes}", allInc);
    replace_all(source, "{static_initializations}", statics);
    replace_all(source, "{registration}", reg);
    replace_all(source, "{initializations}", inits);
    replace_all(source, "{constructor_args}", ctorArgs);
    replace_all(source, "{constructor_body}", ctorBody);
    replace_all(source, "{destructor}", dtor);
    replace_all(source, "{functions}", funcs);

    return source;
}

// Static helper to convert Python-style type names to C++ types
std::string WriterCpp::convertType(const std::string &t) {
    static const std::unordered_map<std::string, std::string> m = {
        {"list", "std::vector"},
        {"map", "std::map"},
        {"string", "std::string"},
        {"Observer", "Observer<std::function<void()>>"}};
    auto it = m.find(t);
    return it != m.end() ? it->second : t;
}

// Build template-arguments string "<...>"
std::string WriterCpp::getTemplates(const Object &obj) {
    std::vector<std::string> parts;
    for (auto &arg : obj.template_args) {
        // Write each template argument as a named object without a name
        std::string s = WriterCpp::writeNamedObject(arg, arg.name, false, true);
        // If it has callable_args, append "(...)" after it
        if (!arg.callable_args.empty() || obj.type == "Observable") {
            std::vector<std::string> callParts;
            for (auto &c : arg.callable_args) {
                callParts.push_back(
                    WriterCpp::writeNamedObject(c, "", true, false));
            }
            s += "(" + join(callParts, ", ") + ")";
        }
        parts.push_back(s);
    }
    if (parts.empty())
        return "";
    return "<" + join(parts, ", ") + ">";
}

// Write a named object, e.g. "const std::string& name"
std::string WriterCpp::writeNamedObject(const Object &obj,
                                        const std::string &name,
                                        bool tryConstRef, bool useIntrusive) {
    auto canUseConstRef = [&](const Object &o) {
        return o.type == "string" || o.type == "list" || o.type == "map";
    };
    bool isRef = obj.is_ref;
    bool is_const = obj.is_const;
    if (!isRef && !is_const && tryConstRef && canUseConstRef(obj)) {
        isRef = true;
        is_const = true;
    }
    std::string baseType = convertType(obj.type);
    std::string templates = getTemplates(obj);
    // handle function pointer-like types "foo(bar)" => expand inner types
    auto posL = baseType.find('(');
    auto posR = baseType.find(')');
    if (posL != std::string::npos && posR != std::string::npos && posR > posL) {
        auto inner = baseType.substr(posL + 1, posR - posL - 1);
        std::vector<std::string> toks;
        std::istringstream iss(inner);
        std::string tok;
        while (std::getline(iss, tok, ',')) {
            auto t = tok;
            t.erase(0, t.find_first_not_of(" \t"));
            t.erase(t.find_last_not_of(" \t") + 1);
            toks.push_back(convertType(t));
        }
        baseType = baseType.substr(0, posL + 1) + join(toks, ", ") +
                   baseType.substr(posR);
    }
    bool usePtr = useIntrusive && obj.is_pointer && !obj.is_const &&
                  !obj.is_link && !obj.denied_intrusive;
    std::string pattern =
        usePtr ? "{static}{const}intrusive_ptr<{type}{templates}>{ref}{name}"
               : "{static}{const}{type}{templates}{pointer}{ref}{name}";
    std::string result = pattern;
    // replacements
    auto rep = [&](const std::string &key, const std::string &val) {
        size_t p = 0;
        while ((p = result.find("{" + key + "}", p)) != std::string::npos) {
            result.replace(p, key.size() + 2, val);
            p += val.size();
        }
    };
    rep("static", obj.is_static ? "static " : "");
    rep("const", is_const || obj.is_link ? "const " : "");
    rep("type", baseType);
    rep("templates", templates);
    rep("pointer", obj.is_pointer || obj.is_link ? "*" : "");
    rep("ref", isRef ? "&" : "");
    rep("name", name.empty() ? "" : " " + name);
    return result;
}

// Member declaration in the class body
std::string WriterCpp::writeMemberDeclaration(const Object &obj) {
    return writeNamedObject(obj, obj.name, false, true) + ";";
}

// Enum member: if name=="value", just a field; otherwise a constexpr BaseEnum
std::string WriterCpp::writeMemberEnumDeclaration(const Object &obj) {
    if (obj.name == "value")
        return writeMemberDeclaration(obj);
    return "static constexpr BaseEnum " + obj.name + " = " + obj.value + ";";
}

// Static member initialization in the CPP
std::string WriterCpp::writeMemberStaticInit(const Class &cls,
                                             const Object &obj) {
    bool useIntr = true;
    std::string tpl = getTemplates(obj);
    std::string init = obj.value.empty() ? "" : "(" + obj.value + ")";
    std::string patPtr =
        "{const}intrusive_ptr<{type}{templates}> {owner}::{name}{initial}"
        ";";
    std::string patNon =
        "{const}{type}{templates}{pointer} {owner}::{name}{initial}"
        ";";
    std::string pat = (useIntr && obj.is_pointer && !obj.is_const &&
                       !obj.is_link && !obj.denied_intrusive)
                          ? patPtr
                          : patNon;
    // fill
    auto fill = [&](const std::string &key, const std::string &val) {
        size_t p = 0;
        while ((p = pat.find("{" + key + "}", p)) != std::string::npos) {
            pat.replace(p, key.size() + 2, val);
            p += val.size();
        }
    };
    fill("const", (obj.is_const || obj.is_link) ? "const " : "");
    fill("type", convertType(obj.type));
    fill("templates", tpl);
    fill("pointer", (obj.is_pointer || obj.is_link) ? "*" : "");
    fill("owner", cls.name);
    fill("name", obj.name);
    fill("initial", init);
    return pat;
}

// Member initialization in ctor initializer list
std::string WriterCpp::writeMemberInitialization(const Object &obj) {
    auto val = obj.value.empty() ? "" : obj.value;
    return obj.name + "(" + val + ")";
}

// Function HPP signature
std::string WriterCpp::writeFunctionHpp(const Function &method) {
    if (currentClass_->name == "TestCase" && method.name == "assertEqual") {
        std::cout << "";
    }
    // pattern: "{virtual}{static}{friend}{type}
    // {name}({args}){const}{override}{abstract}"
    std::string pat = "{virtual}{friend}{templates}{static}{type} "
                      "{name}({args}){const}{override}{abstract}";
    // return type
    std::string ret = "";
    if (method.name != currentClass_->name)
        ret = writeNamedObject(method.return_type, "", false, true);
    // args
    std::string args = createFunctionHppArgs(method);
    // qualifiers
    std::string virt = (method.is_virtual || method.is_abstract ||
                        currentClass_->is_virtual) &&
                               method.name != currentClass_->name
                           ? "virtual "
                           : "";
    std::string stat = method.is_static ? " static " : "";
    std::string frnd = method.is_friend ? "friend " : "";
    std::string cst = method.is_const ? " const" : "";
    std::string ovrd = isOverride(method, currentClass_) ? " override" : "";
    std::string absr = method.is_abstract ? " = 0" : "";
    std::string tmpl;

    if (!method.template_args.empty()) {
        std::string ss;
        ss += "template <";
        for (size_t i = 0; i < method.template_args.size(); ++i) {
            ss += "class " + method.template_args[i].type;
            if (i != method.template_args.size() - 1) {
                ss += ", ";
            }
        }
        ss += ">";
        tmpl = ss;

        if (method.specific_implementations.empty())
            pat += "\n{\n{body}\n}\n";
        else
            pat += ";\n";
    } else {
        pat += ";\n";
    }
    // fill
    replace_all(pat, "{virtual}", virt);
    replace_all(pat, "{static}", stat);
    replace_all(pat, "{friend}", frnd);
    replace_all(pat, "{type}", ret);
    replace_all(pat, "{name}", method.name);
    replace_all(pat, "{args}", args);
    replace_all(pat, "{const}", cst);
    replace_all(pat, "{override}", ovrd);
    replace_all(pat, "{abstract}", absr);
    replace_all(pat, "{templates}", tmpl);
    replace_all(pat, "{body}", method.body);
    return pat;
}

// Function CPP definition
std::string WriterCpp::writeFunctionCpp(const Function &method) {
    if (!method.specific_implementations.empty())
        return method.specific_implementations;
    if (method.is_external || method.is_abstract || !method.template_args.empty())
        return "";

    // pattern:
    std::string pat = "{type} {scope}{name}({args}){const}\n{\n{body}\n}\n\n";
    std::string ret = "", scope = "", args = "";
    if (method.name != currentClass_->name)
        ret = writeNamedObject(method.return_type, "", false, true);
    if (!method.is_friend)
        scope = currentClass_->name + "::";
    args = createFunctionCppArgs(method);
    std::string cst = method.is_const ? " const" : "";
    // fill
    auto fill = [&](const std::string &key, const std::string &val) {
        size_t p = 0;
        while ((p = pat.find("{" + key + "}", p)) != std::string::npos) {
            pat.replace(p, key.size() + 2, val);
            p += val.size();
        }
    };
    fill("type", ret);
    fill("scope", scope);
    fill("name", method.name);
    fill("args", args);
    fill("const", cst);
    fill("body", strip(method.body));
    return pat;
}

// Build HPP argument list "T a, U b=val"
std::string WriterCpp::createFunctionHppArgs(const Function &method) {
    std::vector<std::string> parts;
    for (auto &arg : method.callable_args) {
        auto s = writeNamedObject(arg, arg.name, true, false);
        if (!arg.value.empty())
            s += "=" + arg.value;
        parts.push_back(s);
    }
    return join(parts, ", ");
}

// Build CPP argument list
std::string WriterCpp::createFunctionCppArgs(const Function &method) {
    std::vector<std::string> parts;
    for (auto &arg : method.callable_args) {
        parts.push_back(writeNamedObject(arg, arg.name, true, false));
    }
    return join(parts, ", ");
}

// Determine override
bool WriterCpp::isOverride(const Function &method,
                           const std::shared_ptr<Class> &cls) {
    if (!method.is_virtual)
        return false;
    if(cls->name == "Foo2" && method.name == "virtual_method2")
        std::cout << "";
    static const std::set<std::string> allowed = {"visit"};
    auto supName = cls->parent_class_name;
    while (!supName.empty()) {
        auto sup = _model->get_class(supName);
        for (auto &fn : sup->functions) {
            if (fn.name != method.name)
                continue;
            // return-type check
            if (fn.return_type.type != method.return_type.type) {
                // validation
                if (!_model->config.allow_different_virtual)
                    Error::exit(
                        Error::ERROR_VIRTUAL_METHOD_HAS_DIFFERENT_DECLARATION,
                        cls->name, method.name, supName);
                continue;
            }
            // args count check
            if (fn.callable_args.size() != method.callable_args.size() &&
                !allowed.count(method.name)) {
                if (!_model->config.allow_different_virtual)
                    Error::exit(
                        Error::ERROR_VIRTUAL_METHOD_HAS_DIFFERENT_DECLARATION,
                        cls->name, method.name, supName);
                continue;
            }
            // each arg type
            bool cont = false;
            for (size_t i = 0; i < fn.callable_args.size(); ++i) {
                if (fn.callable_args[i].type != method.callable_args[i].type) {
                    if (!_model->config.allow_different_virtual &&
                        !allowed.count(method.name)) {
                        Error::exit(
                            Error::
                                ERROR_VIRTUAL_METHOD_HAS_DIFFERENT_DECLARATION,
                            cls->name, method.name, supName);
                    }
                    cont = true;
                    break;
                }
            }
            if (cont)
                continue;
            return true;
        }
        supName = sup->parent_class_name;
    }
    return false;
}

// Include resolution for header
std::tuple<std::set<std::string>, std::set<std::string>, std::set<std::string>>
WriterCpp::getIncludesForHeader(const std::shared_ptr<Class> &cls) {
    std::set<std::string> inc, fwd, fwdOut;

    std::function<void(std::set<std::string> &, const Object &)> addObj =
        [&](std::set<std::string> &container, const Object &o) {
            container.insert(convertType(o.type));
            for (auto &ta : o.template_args) {
                addObj(fwd, ta);
            }
            for (auto &ca : o.callable_args) {
                addObj(fwd, ca);
            }
        };
    std::function<void(std::set<std::string> &, const Object &)> parseObj =
        [&](std::set<std::string> &container, const Object &o) {
            addObj(container, o);
            for (auto &arg : o.template_args) {
                if (cls->prefer_use_forward_declarations)
                    parseObj(fwd, arg);
                else
                    parseObj(container, arg);
            }
        };

    // members
    for (auto &m : cls->members) {
        if(_model->is_skip(m))
            continue;
        parseObj(inc, m);
        if (m.type.find("std::atomic") != std::string::npos)
            inc.insert("std::atomic");
    }
    // functions
    static const std::set<std::string> stdIns = {"map", "list", "string"};
    static std::vector<std::string> mg_extensions = {
        "in_map",
        "in_list",
        "list_push",
        "list_insert",
        "list_remove",
        "list_erase",
        "list_truncate",
        "list_size",
        "list_index",
        "list_clear",
        "list_resize",
        "map_size",
        "map_clear",
        "map_remove",
        "string_empty",
        "string_size",
        "random_float",
        "random_int",
        "mg_swap",
        "split",
        "join",
        "strTo",
        "toStr",
        "serialize_command_to_xml",
        "create_command_from_xml",
        "serialize_command_to_json",
        "create_command_from_json",
        "clone_object",
        "fs_get_string",
        "Default",
        "format",
    };

    auto add_from_method = [&](const Function &fn) {
        for (auto &arg : fn.callable_args) {
            auto &t = arg.type;
            if (stdIns.count(t))
                addObj(inc, arg);
            else if (t.rfind("pugi::", 0) == 0 || t.rfind("Json::", 0) == 0)
                addObj(fwdOut, arg);
            else
                addObj(fwd, arg);
        }
        auto &rt = fn.return_type.type;
        if (!_model->config.user_includes || stdIns.count(rt))
            addObj(inc, fn.return_type);
        else
            addObj(fwd, fn.return_type);

        if (!fn.template_args.empty() || fn.is_template) {
            for (auto &func : mg_extensions) {
                if (fn.body.find(func) != std::string::npos) {
                    inc.insert("mg_extensions");
                    break;
                }
            }
        }
    };
    for (auto &fn : cls->functions) {
        if(_model->is_skip(fn))
            continue;
        add_from_method(fn);
    }
    for (auto &fn : cls->constructors) {
        if(_model->is_skip(fn))
            continue;
        add_from_method(fn);
    }
    // superclasses
    if (!cls->parent_class_name.empty())
        inc.insert(cls->parent_class_name);
    // remove self
    inc.erase(cls->name);
    fwd.erase(cls->name);
    fwdOut.erase(cls->name);
    return {inc, fwd, fwdOut};
}

// Include resolution based on method bodies
std::set<std::string>
WriterCpp::getIncludesForMethod(const Model& model,
                                const std::shared_ptr<Class> &cls,
                                const std::string &text,
                                const std::set<std::string> &hppInc) {
    std::set<std::string> out;
    for (auto &cls : model.classes) {
        if(!cls || model.is_skip(*cls)){
            continue;
        }
        auto &n = cls->name;
        if (!hppInc.count(n) && text.find(n) != std::string::npos) {
            if (text.find(cls->name) != std::string::npos) {
                out.insert(cls->name);
            }
        }
    }
    return out;
}

// Build includes for source
std::string WriterCpp::getIncludesForSource(const std::shared_ptr<Class> &cls, const std::string &funcText, std::set<std::string> hppInc, const std::set<std::string> &fwd, const std::set<std::string> &fwdOut) {
    
    std::set<std::string> inc = hppInc;
    inc.insert(cls->name);
    inc.insert(fwd.begin(), fwd.end());
    inc.insert(fwdOut.begin(), fwdOut.end());
    auto more = getIncludesForMethod(*_model, cls, funcText, hppInc);
    inc.insert(more.begin(), more.end());
    for(auto& name : cls->user_includes){
        auto cls = _model->get_class(name);
        if(!cls || _model->is_skip(*cls)){
            continue;
        }
        inc.insert(name);
    }
    return buildIncludes(cls, inc);
}

// Build "#include <...>" or "#include \"...\"" lines
std::string WriterCpp::buildIncludes(const std::shared_ptr<Class> &cls,
                                     const std::set<std::string> &incs) {
    const std::unordered_map<std::string, std::string> mapTpl = {
        {"std::vector", "<vector>"},
        {"std::map", "<map>"},
        {"std::set", "<set>"},
        {"std::string", "<string>"},
        {"std::tuple", "<tuple>"},
        {"std::atomic", "<atomic>"},
        {"Json::Value", "\"jsoncpp/json.h\""},
        {"pugi::xml_node", "\"pugixml/pugixml.hpp\""},
        {"Observer", "\"Observer.h\""},
        {"intrusive_ptr", "\"intrusive_ptr.h\""},
        {"ecs_helper", "\"" + getPathToRoot(cls) + "ecs_helper.h\""},
        {"mg_extensions", "\"" + getPathToRoot(cls) + "mg_extensions.h\""},
    };
    std::vector<std::string> lines;
    for (auto &t : incs) {
        auto it = mapTpl.find(t);
        if (it != mapTpl.end()) {
            lines.push_back("#include " + it->second);
        } else if (_model->hasClass(t)) {
            auto other = _model->get_class(t);
            std::string path = "\"";
            if (!cls->group.empty() && cls->group != other->group)
                path += "../";
            if (!other->group.empty() && cls->group != other->group)
                path += other->group + "/";
            path += other->name + ".h\"";
            lines.push_back("#include " + path);
        }
    }
    std::sort(lines.begin(), lines.end());
    return join(lines, "\n");
}

// Forward declarations
std::string
WriterCpp::buildForwardDeclarations(const std::set<std::string> &decls) {
    static const std::set<std::string> ignore = {
        "std::vector", "std::map", "std::set", "std::string", "int",
        "bool",        "float",    "void",     "Observable"};
    static const std::unordered_map<std::string, std::string> predefined = {
        {"Json::Value", "namespace Json { class Value; }"},
        {"pugi::xml_node", "namespace pugi { class xml_node; }"}};
    std::vector<std::string> out;
    for (auto &d : decls) {
        if (ignore.count(d))
            continue;
        auto it = predefined.find(d);
        if (it != predefined.end()) {
            out.push_back(it->second);
        } else if (_model->hasClass(d)) {
            out.push_back("class " + d + ";");
        }
    }
    std::sort(out.begin(), out.end());
    return join(out, "\n");
}

// Filename helpers
std::string WriterCpp::getFilename(const std::shared_ptr<Class> &cls,
                                   const std::string &ext) {
    std::string fn = cls->name + "." + ext;
    if (!cls->group.empty())
        fn = cls->group + "/" + fn;
    return fn;
}

std::string WriterCpp::getPathToRoot(const std::shared_ptr<Class> &cls) {
    return cls->group.empty() ? "" : "../";
}
