//
//  GeneratorBindings.hpp
//  mlc-cpp
//

#pragma once

#include "FeatureGenerator.hpp"
#include "../utils/Config.hpp"
#include <memory>
#include <string>
#include <vector>

class Class;
class Function;
class Model;
class Object;

class GeneratorBindings : public FeatureGenerator {
public:
    GeneratorBindings(const FeatureBindings& config);
    void generate(Model &model) override;
    void modifySources(Model &model, const std::shared_ptr<Class> &cls, std::string &header, std::string &source) override;

public:
    struct BindingEntry {
        std::shared_ptr<Class> cls;
        const Function *fn;
    };

    struct ObservableEntry {
        std::shared_ptr<Class> cls;
        const Object *member;
    };

    std::vector<BindingEntry> collectBindings(Model &model) const;
    std::vector<ObservableEntry> collectObservableBindings(Model &model) const;
    std::string renderBindingClassHeader(const Model &model, const std::vector<BindingEntry> &entries) const;
    std::string renderBindingFuncHeader(const Model &model, const std::vector<BindingEntry> &entries) const;
    std::string renderBindingFuncSource(const Model &model,
                                        const std::vector<BindingEntry> &entries,
                                        const std::vector<ObservableEntry> &observables) const;
    bool isSupported(const Class &cls, const Function &fn) const;
    static std::string sanitize(const std::string &value);
private:
    const FeatureBindings& _config;
};
