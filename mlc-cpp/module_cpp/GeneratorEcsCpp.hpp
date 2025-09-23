//
//  GeneratorEcsCpp.hpp
//  mlc-cpp
//
//  C++ port of plugin/ecs_generate.py
//

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Model.hpp"

class Model;
struct Class;
struct Function;
class Object;

class GeneratorEcsCpp : public CustomGenerator {
public:
    virtual void generate(Model &model) override;
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) override;

private:
    // helpers
    static bool isBased(const std::shared_ptr<Class>& cls, const std::string &name);
    static std::string componentsField(const std::shared_ptr<Class>& cls); // ComponentFoo -> foo
    static std::shared_ptr<Class> getClass(Model &model, const std::string &name);
    static Function makeFnDecl(const std::string &decl);
    static Object   makeObj(const std::string &decl, bool withName=false);

    // generation steps
    void generateContainersAndClear(Model &model, const std::shared_ptr<Class>& ecsBase);
    void generateSystemsEndTurn(Model &model);
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

    std::vector<std::shared_ptr<Class>> getComponentClasses(Model &model);

    void addHelperFile(Model &model);
};

