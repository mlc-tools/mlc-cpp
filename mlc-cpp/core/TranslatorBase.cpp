//
//  TranslatorBase.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#include "TranslatorBase.hpp"
#include "Model.hpp"
#include "Class.hpp"
#include "Function.hpp"
#include "Object.hpp"
#include "Common.hpp"
//#include "Error.hpp"

//#include <fmt/core.h>  // если доступно, можно заменить на std::ostringstream

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
    }
}

void TranslatorBase::translateFunction(Class &cls,
                                       Function &method,
                                       Model &model)
{
    if (!method.translated) {
        method.body = translateFunctionBody(cls, method, method.body, model, method.callable_args);
    }
}

std::vector<int> TranslatorBase::convertToEnum(Class &cls) {
    int shift = 0;
    std::string castType = "string";
    std::vector<int> values;
    for (auto &member : cls.members) {
        if (!member.name.empty()) continue;
        member.name     = member.type;
        member.type     = castType;
        member.is_static = true;
        member.is_const  = true;
        if (member.value.empty()) {
            if (castType == "int") {
                int v = 1 << shift;
                member.value = "(" + std::to_string(v) + ")";
                values.push_back(v);
            } else {
                member.value = "\"" + member.name + "\"";
            }
        } else {
            castType = "int";
            values.push_back(std::stoi(member.value));
        }
        ++shift;
    }
    return values;
}

std::string TranslatorBase::replacePattern(
    const std::string &text,
    const std::tuple<std::regex,std::string,std::vector<std::string>> &pattern)
{
    bool skip = std::get<2>(pattern).size() > 0;
    for (auto &f : std::get<2>(pattern)) {
        skip = skip && (text.find(f) == std::string::npos);
    }
    if (!skip) {
        return std::regex_replace(text,
                                  std::get<0>(pattern),
                                  std::get<1>(pattern));
    }
    return text;
}

std::pair<std::string,std::vector<std::string>>
TranslatorBase::saveStrings(std::string func) {
    std::vector<std::string> strings;
    while (true) {
        auto pos = func.find('"');
        if (pos == std::string::npos) break;
        size_t i = pos + 1; char prev = 0;
        for (; i < func.size(); ++i) {
            if (func[i]=='"' && prev!='\\') {
                std::string lit = func.substr(pos, i-pos+1);
                std::string token = "@{{__string_" + to_string(strings.size()) + "__}}";
                func = func.substr(0,pos) + token + func.substr(i+1);
                strings.push_back(lit);
                break;
            }
            prev = func[i];
        }
    }
    return {func, strings};
}

std::string TranslatorBase::restoreStrings(std::string func,
                                           const std::vector<std::string> &strings)
{
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
