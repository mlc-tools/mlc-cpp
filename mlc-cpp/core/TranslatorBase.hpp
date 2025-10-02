//
//  TranslatorBase.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#pragma once

#include <memory>
#include <re2/re2.h>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

class Model;
class Class;
class Function;
class Object;
struct RegexPattern;

/// Базовый класс для трансляции тела методов (инлайн-замены по regex,
/// enum-конвертация и т.д.).
class TranslatorBase {
public:
    TranslatorBase() = default;
    virtual ~TranslatorBase() = default;

    /// Основной entrypoint: пройтись по всем классам модели и перевести всё
    /// нужное.
    virtual void translate(Model &model);

public:
    /// Обработать один метод: собрать body из operations и вызвать
    /// translateFunctionBody.
    virtual void translateFunction(Class &cls, Function &method, Model &model);

    /// Собственно место для ваших замен: по умолчанию вызывает replaceByRegex.
    virtual std::string translateFunctionBody(Class &cls, Function &method,
                                              const std::string &body,
                                              Model &model,
                                              const std::vector<Object> &args);

    /// Реализуйте в наследнике все нужные regex-замены.
    virtual std::string replaceByRegex(const std::string &body, Class &cls,
                                       Function &method, Model &model,
                                       const std::vector<Object> &args) = 0;

    /// Конвертировать enum-класс: заполнить имя/тип/initial_value, вернуть
    /// список значений.
    virtual std::vector<int> convertToEnum(Class &cls);

    virtual void replacePattern(std::string &text, const RegexPattern &pattern);

    virtual std::pair<std::string, std::vector<std::string>>
    saveStrings(std::string func);

    virtual std::string restoreStrings(std::string func,
                                       const std::vector<std::string> &strings);

protected:
    Model *_model;
};
