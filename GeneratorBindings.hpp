//
//  GeneratorBindings.hpp
//  mlc-cpp
//

#pragma once

#include "FeatureGenerator.hpp"
#include <memory>
#include <string>
#include <vector>

class Class;
class Function;
class Model;

class GeneratorBindings : public FeatureGenerator {
public:
    void generate(Model &model) override;
    void modifySources(Model &model, const std::shared_ptr<Class> &cls,
                       std::string &header, std::string &source) override;

private:
    struct BindingEntry {
        std::shared_ptr<Class> cls;
        const Function *fn;
    };

    std::vector<BindingEntry> collectBindings(Model &model) const;
    std::string renderHeader(const Model &model,
                             const std::vector<BindingEntry> &entries) const;
    bool isSupported(const Class &cls, const Function &fn) const;
    static std::string sanitize(const std::string &value);
};
