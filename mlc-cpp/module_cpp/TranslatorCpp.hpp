//
//  TranslatorCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//
#include "TranslatorBase.hpp"

/// C++-реализация Translator, наследника TranslatorBase.
/// Делает инлайн-regex-замены и расширенный enum-генератор.
class TranslatorCpp : public TranslatorBase {
public:
    TranslatorCpp() = default;
    ~TranslatorCpp() override = default;

    /// Переопределяем перевод всей модели, чтобы вызвать нашу логику
    virtual void translate(Model &model) override;

protected:
    /// Переопределяем обработку одного метода
    virtual void translateFunction(Class &cls,
                           Function &method,
                           Model &model) override;

    /// Переопределяем только тело метода
    virtual std::string translateFunctionBody(Class &cls,
                                      Function &method,
                                      const std::string &body,
                                      Model &model,
                                      const std::vector<Object> &args) override;

    /// Расставляем все regex-паттерны из RegexPatternCpp
    virtual std::string replaceByRegex(const std::string &body,
                               Class &cls,
                               Function &method,
                               Model &model,
                               const std::vector<Object> &args) override;

    /// Специальная логика генерации enum: добавляем конструкторы и операторы
    virtual std::vector<int> convertToEnum(Class &cls) override;
};

