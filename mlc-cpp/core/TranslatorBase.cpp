//
//  TranslatorBase.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#include "TranslatorBase.hpp"
#include "Class.hpp"
#include "Common.hpp"
#include "Error.hpp"
#include "Function.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "RegexPatternCpp.hpp"

void TranslatorBase::translate(Model &model) {
    _model = &model;

    for (auto &clsPtr : model.classes) {
        Class &cls = *clsPtr;
        // пометить runtime-поля для Observable
        for (auto &member : cls.members) {
            if (member.type == "Observable") {
                member.is_runtime = true;
            } else {
                for (auto &t : member.template_args) {
                    if (t.type == "Observable") {
                        member.is_runtime = true;
                        break;
                    }
                }
            }
        }
        // если enum — конвертировать
        if (cls.type == "enum") {
            convertToEnum(cls);
        }
        // перевод методов
        for (auto &method : cls.functions) {
            translateFunction(cls, method, model);
        }
        for (auto &method : cls.constructors) {
            translateFunction(cls, method, model);
        }
    }
}

void TranslatorBase::translateFunction(Class &cls, Function &method,
                                       Model &model) {
    if (!method.translated) {
        method.body = translateFunctionBody(cls, method, method.body, model,
                                            method.callable_args);
    }
}

std::string
TranslatorBase::translateFunctionBody(Class &cls, Function &method,
                                      const std::string &body, Model &model,
                                      const std::vector<Object> &args) {
    auto result = replaceByRegex(body, cls, method, model, args);
    for (auto &inner : cls.inner_classes) {
        auto original_name = inner->name.substr(cls.name.size());
        if (body.find(original_name) != std::string::npos) {
            RE2 regex("\\b" + original_name + "\\b");
            RE2::GlobalReplace(&result, regex, inner->name);
        }
    }
    return result;
}

void TranslatorBase::convertToEnum(Class &cls) {
    cls.parent_class_name = "BaseEnum";
    cls.parent = _model->get_class(cls.parent_class_name);

    int shift = 0;
    std::string castType = "string";
    std::vector<std::string> values;
    for (auto &member : cls.members) {
        if (!member.name.empty())
            continue;
        member.name = member.type;
        member.type = castType;
        member.is_static = true;
        member.is_const = true;
        if (member.value.empty()) {
            if (castType == "int") {
                int v = 1 << shift;
                member.value = "(" + std::to_string(v) + ")";
                values.push_back(to_string(v));
            } else {
                member.value = "\"" + member.name + "\"";
            }
        } else {
            castType = "int";
            values.push_back(member.value);
        }
        ++shift;
    }
}

void TranslatorBase::replacePattern(std::string &text,
                                    const RegexPattern &pattern) {
    bool skip = pattern.triggers.size() > 0;
    for (auto &f : pattern.triggers) {
        skip = skip && (text.find(f) == std::string::npos);
    }
    if (!skip) {
        RE2::GlobalReplace(&text, *pattern.pattern, pattern.replacement);
    }
}

std::pair<std::string, std::vector<std::string>>
TranslatorBase::saveStrings(std::string func) {
    std::vector<std::string> strings;
    while (true) {
        auto pos = func.find('"');
        if (pos == std::string::npos)
            break;
        size_t i = pos + 1;
        char prev = 0;
        for (; i < func.size(); ++i) {
            if (func[i] == '"' && prev != '\\') {
                std::string lit = func.substr(pos, i - pos + 1);
                std::string token =
                    "@{{__string_" + to_string(strings.size()) + "__}}";
                func = func.substr(0, pos) + token + func.substr(i + 1);
                strings.push_back(lit);
                break;
            }
            prev = func[i];
        }
    }
    return {func, strings};
}

std::string
TranslatorBase::restoreStrings(std::string func,
                               const std::vector<std::string> &strings) {
    for (size_t i = 0; i < strings.size(); ++i) {
        std::string token = "@{{__string_" + to_string(i) + "__}}";
        size_t pos = 0;
        while ((pos = func.find(token, pos)) != std::string::npos) {
            func.replace(pos, token.size(), strings[i]);
            pos += strings[i].size();
        }
    }
    return func;
}
