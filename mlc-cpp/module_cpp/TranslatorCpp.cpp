//
//  TranslatorCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#include "TranslatorCpp.hpp"
#include "Model.hpp"
#include "Class.hpp"
#include "Function.hpp"
#include "Object.hpp"
#include "Common.hpp"
#include "RegexPatternCpp.hpp"
#include <iostream>

#include <regex>

void TranslatorCpp::translate(Model &model) {
    // просто вызываем базовый, он пройдёт по всем классам
    TranslatorBase::translate(model);
}

void TranslatorCpp::translateFunction(Class &cls,
                                   Function &method,
                                   Model &model) {
    if (!method.translated) {
        method.body = translateFunctionBody(cls, method, method.body, model, method.callable_args);
    }
}

std::string TranslatorCpp::translateFunctionBody(Class &cls,
                                              Function &method,
                                              const std::string &body,
                                              Model &model,
                                              const std::vector<Object> &args)
{
    auto result = replaceByRegex(body, cls, method, model, args);
    for(auto& inner : cls.inner_classes){
        auto original_name = inner->name.substr(cls.name.size());
        if(body.find(original_name) != std::string::npos){
            auto regex = std::regex("\\b" + original_name + "\\b");
            result = std::regex_replace(result, regex, inner->name);
        }
    }
    return result;
}

std::string TranslatorCpp::replaceByRegex(const std::string &body,
                                       Class &cls,
                                       Function &method,
                                       Model &model,
                                       const std::vector<Object> &args)
{
    // Сохраняем строковые литералы
    auto pair = saveStrings(body);
    auto tmp = pair.first;
    auto strings = pair.second;
    
    if(cls.name == "TestTranslates" && method.name == "test_list_do")
    {
        std::cout << "";
    }

    // Функции-замены
    for (auto &pat : RegexPatternCpp::functionPatterns) {
        tmp = replacePattern(tmp, {pat.pattern, pat.replacement, pat.triggers});
    }
    // Простые replaces
    for (auto &pr : RegexPatternCpp::replaces) {
        replace_all(tmp, pr.first, pr.second);
    }
    // C++17 -> C++14
    for (auto &pat : RegexPatternCpp::convertC17toC14) {
        tmp = replacePattern(tmp, {pat.pattern, pat.replacement, pat.triggers});
    }

    // Восстанавливаем литералы
    tmp = restoreStrings(tmp, strings);
    return tmp;
}

std::vector<int> TranslatorCpp::convertToEnum(Class &cls) {
    
    cls.parent_class_name = "BaseEnum";
    cls.parent = _model->get_class(cls.parent_class_name);
    
    // Практически дословно из Python-реализации
    bool isNumeric = cls.is_numeric; // предполагается поле Class::isNumeric
    int shift = 0;
    std::vector<int> values;
    const std::string cast = "int";

    // 1) переименовываем члены, собираем значения
    for (auto &member : cls.members) {
        if (!member.name.empty()) continue;
        member.name     = member.type;
        member.type     = cast;
        member.is_static = true;
        member.is_const  = true;
        if (member.value.empty()) {
            int v = isNumeric ? shift : (1 << shift);
            member.value = isNumeric
                ? std::to_string(shift)
                : "(" + std::to_string(v) + ")";
            values.push_back(v);
        } else {
            int v = std::stoi(member.value);
            values.push_back(v);
        }
        ++shift;
    }

    // Вспомогалки для удобства добавления функций
    auto addMethod = [&](const Object& ret,
                         const std::string &name,
                         bool isConst,
                         bool isFriend = false)
    {
        cls.functions.emplace_back();
        Function &m = cls.functions.back();
        m.return_type = ret;
        m.name       = name;
        m.is_const    = isConst;
        m.is_friend   = isFriend;
        return &m;
    };

    auto makeConstRef = [&](const std::string &t, const std::string& name=""){
        Object o;
        o.type     = t;
        o.name = name;
        o.is_const  = true;
        o.is_ref    = true;
        return o;
    };

    // 2) добавляем конструкторы и операторы
    //   Ctor(int)
    {
        auto method = addMethod(Objects::VOID, cls.name, false);
        method->callable_args.push_back(Object("int", "_value"));
        method->body += "value = _value;";
    }
    //   Ctor(const Enum&)
    {
        auto method = addMethod(Objects::VOID, cls.name, false);
        method->callable_args.push_back(makeConstRef(cls.name, "rhs"));
        method->body += "value = rhs.value;";
    }
    //   Ctor(const std::string&)
    {
        auto method = addMethod(Objects::VOID, cls.name, false);
        method->callable_args.push_back(Object("string", "_value"));
        for (size_t i = 0; i < cls.members.size() && i < values.size(); ++i) {
            const auto &mb = cls.members[i];
            if (mb.name != "value") {
                method->body += format_indexes("\nif(_value == \"{0}\") \n{ \nvalue = {0}; \nreturn; \n}", mb.name);
            }
        }
        method->body += "\nvalue = 0;";
    }
    //   operator=(const Enum&)
    {
        auto ret = makeConstRef(cls.name, "");
        auto method = addMethod(ret, "operator =", false);
        method->callable_args.push_back(Object(ret.type, "rhs"));
        method->body += "value = rhs.value;";
        method->body += "return *this;";
    }
    //   operator=(int)
    {
        auto ret = makeConstRef(cls.name, "");
        auto method = addMethod(ret, "operator =", false);
        method->callable_args.push_back(Object("int", "rhs"));
        method->body += "value = rhs;";
        method->body += "return *this;";
    }
    //   operator=(const std::string&)
    {
        auto ret = makeConstRef(cls.name);
        auto method = addMethod(ret, "operator =", false);
        method->callable_args.push_back(Object("string", "_value"));
        for (size_t i = 0; i < cls.members.size() && i < values.size(); ++i) {
            const auto &mb = cls.members[i];
            if (mb.name != "value") {
                method->body += format_indexes("\nif(_value == \"{0}\") \n{\n value = {0}; \nreturn *this; \n}", mb.name);
            }
        }
        method->body += "return *this;";
    }
    //   operator==(BaseEnum)
    {
        auto method = addMethod(Objects::BOOL, "operator ==", true);
        method->callable_args.push_back(makeConstRef(cls.parent_class_name, "rhs"));
        method->body += "return value == rhs.operator int();";
    }
    //   operator==(Enum)
    {
        auto method = addMethod(Objects::BOOL, "operator ==", true);
        method->callable_args.push_back(makeConstRef(cls.name, "rhs"));
        method->body += "return value == rhs.value;";
    }
    //   operator==(int)
    {
        auto method = addMethod(Objects::BOOL, "operator ==", true);
        method->callable_args.push_back(Object("int", "rhs"));
        method->body += "return value == rhs;";
    }
    //   operator==(const std::string&)
    {
        auto method = addMethod(Objects::BOOL, "operator ==", true);
        method->callable_args.push_back(Object("string", "rhs"));
        method->body += "return " + cls.name + "(rhs) == rhs;";  // simple delegating;
    }
    //   friend operator==(const std::string&, const Enum&)
    {
        auto method = addMethod(Objects::BOOL, "operator ==", false, /*friend*/ true);
        method->callable_args.push_back(Object("string", "lhs"));
        method->callable_args.push_back(makeConstRef(cls.name, "rhs"));
        method->body += format_indexes("return {0}(lhs) == rhs;", cls.name);
    }
    //   operator<(Enum)
    {
        auto method = addMethod(Objects::BOOL, "operator <", true);
        method->callable_args.push_back(makeConstRef(cls.name, "rhs"));
        method->body += "return value < rhs.value;";
    }
    //   operator int()
    {
        auto method = addMethod(Object(), "operator int", true);
        method->body += "return value;";
    }
    //   operator std::string()
    {
        auto method = addMethod(Object(), "operator std::string", true);
        for (size_t i = 0; i < cls.members.size() && i < values.size(); ++i) {
            const auto &mb = cls.members[i];
            if (mb.name != "value") {
                method->body += format_indexes("\nif(value == {0}) \n{\n return \"{0}\"; \n}", mb.name);
            }
        }
        method->body += "return std::string();";
    }
    //   str()
    {
        auto method = addMethod(Objects::STRING, "str", true);
        for (size_t i = 0; i < cls.members.size() && i < values.size(); ++i) {
            const auto &mb = cls.members[i];
            if (mb.name != "value") {
                method->body += format_indexes("\nif(value == {0}) \n{\n return \"{0}\"; \n}", mb.name);
            }
        }
        method->body += "return std::string();";
    }

    return values;
}
