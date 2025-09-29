//
//  GeneratorDataStorageBase.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#ifndef GeneratorDataStorageBase_hpp
#define GeneratorDataStorageBase_hpp

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cctype>
#include <sstream>
#include "../models/Class.hpp"
#include "features/FeatureGenerator.hpp"

class Model;

class GeneratorDataStorageBase : public FeatureGenerator
{
public:
    GeneratorDataStorageBase();
    virtual void generate(Model &model) override;
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) override{}

protected:
    // должны перегружаться в наследниках
    virtual std::string getInitializeFunctionJsonBody() = 0;
    virtual std::string getInitializeFunctionXmlBody()  = 0;
    virtual std::string getSharedMethodBody()           = 0;
    virtual std::string getPatternGetter()              = 0;

    virtual bool isNeedCreateStaticInstance() const { return true; }

    virtual void addInitializeFunctionXml();
    virtual void addInitializeFunctionJson();
    virtual void createSharedMethod();
    virtual void createGetters(const std::vector<std::shared_ptr<Class>>& classes);
    virtual void createKeysGetter(const std::string& mapName);
    virtual void createGettersMaps(const std::vector<std::shared_ptr<Class>>& classes);
protected:
    std::unordered_map<std::string, Object> dataMembers_;
    Model *_model;
    std::shared_ptr<Class> _class;
};


std::string getDataName(const std::string& name);
std::string getClassNameFromDataName(const std::string& name);
std::string getDataListName(const std::string& name);



#endif /* GeneratorDataStorageBase_hpp */
