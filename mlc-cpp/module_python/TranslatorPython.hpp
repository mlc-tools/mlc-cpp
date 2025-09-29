//
//  TranslatorPython.hpp
//

#pragma once

#include "../core/TranslatorBase.hpp"

class TranslatorPython : public TranslatorBase {
public:
    TranslatorPython() = default;
    ~TranslatorPython() = default;

    void translateFunction(Class &cls, Function &method, Model &model) override;

    std::string translateFunctionBody(Class &cls,
                                      Function &method,
                                      const std::string &body,
                                      Model &model,
                                      const std::vector<Object> &args) override;

    std::string replaceByRegex(const std::string &body,
                               Class &cls,
                               Function &method,
                               Model &model,
                               const std::vector<Object> &args) override;

    std::vector<int> convertToEnum(Class &cls) override;

private:
    static std::string addImports(const Class &clsOwner, const std::string &func, const Model &model);
    static std::string removeDoubleEol(const std::string &func);
    static std::string convertBracesToTabs(const std::string &func);
    static std::string convertBraces(const std::string &body);
    static std::string getTabs(int count);
};
