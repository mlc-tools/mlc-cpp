//
//  GeneratorEcsCpp.hpp
//  mlc-cpp
//
//  C++ port of plugin/ecs_generate.py
//

#pragma once

#include "../models/Model.hpp"
#include "../utils/Config.hpp"
#include "FeatureGenerator.hpp"
#include <memory>
#include <string>
#include <vector>

class Model;
struct Class;
struct Function;
class Object;

class GeneratorEcsCpp : public FeatureGenerator {
public:
    GeneratorEcsCpp(const FeatureEcs &config);
    virtual void generate(Model &model) override;
    virtual void modifySources(Model &model, const std::shared_ptr<Class> &cls,
                               std::string &header,
                               std::string &source) override;

private:
    // helpers
    bool isBased(const std::shared_ptr<Class> &cls, const std::string &name);
    static std::string
    componentsField(const std::shared_ptr<Class> &cls); // ComponentFoo -> foo
    static std::shared_ptr<Class> getClass(Model &model,
                                           const std::string &name);
    static Function makeFnDecl(const std::string &decl);
    static Object makeObj(const std::string &decl, bool withName = false);

    // generation steps
    void createPimplClass(Model &model, const std::shared_ptr<Class> &ecsBase);
    void createPimplMember(Model &model, const std::shared_ptr<Class> &ecsBase);
    void generateContainers(Model &model, const std::shared_ptr<Class> &ecsBase);
    void generateClearComponents(Model &model, const std::shared_ptr<Class> &ecsBase);
    void generateSystemsEndTurn(Model &model, const std::shared_ptr<Class> &cls);
    void generateRemoveEntity(Model &model);
    void generateAddModelMethod(Model &model);
    void generateRemoveModelMethod(Model &model);
    void generateGetSelfFromModelMethod(Model &model);
    void generateHasInModel(Model &model);
    void generateBuildMaps(Model &model);
    void generateModelAddComponent(Model &model);
    void generateModelRemoveComponent(Model &model, bool useRawPointer);
    void generateModelGetComponent(Model &model, bool isConst);
    void generateModelCopyEntityFromModel(Model &model);
    void generateModelGetComponents(Model &model, bool isConst);
    void generateModelGetMapComponents(Model &model, bool isConst);
    
    std::vector<std::shared_ptr<Class>> get_skill_components(Model &model);
    void generate_system_skills(Model &model, const std::string &method_name);
    void generate_model_method_save_skills(Model &model);

    std::vector<std::shared_ptr<Class>> getComponentClasses(Model &model);

    void addHelperFile(Model &model);

private:
    Model *_model;
    std::string _ecs_model_base_name;
    std::string _ecs_component_base_name;
};
