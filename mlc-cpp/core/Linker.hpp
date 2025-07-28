//
//  Linker.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#pragma once
#include "Class.hpp"
#include "Model.hpp"
#include <vector>
#include <memory>


class Linker {
public:
    Linker() = default;
    
    void link(Model &model);

private:
    static void convertSuperclasses(Model &model);
    static void convertTemplates(Model &model);
    static void generateInlineClasses(Model &model);
    static void addGetTypeMethod(Model &model);

    static void processTemplates(Object &member, Model &model);
    static std::shared_ptr<Object> getObjectType(Model &model, const std::string &typeName);
    static void generateInlineFor(Class &cls);
};

