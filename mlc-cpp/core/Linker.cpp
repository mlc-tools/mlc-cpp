//
//  Linker.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Linker.hpp"
#include <cassert>


void Linker::link(Model &model) {
    for(auto& cls : model.classes){
        model.classesDict[cls->name] = cls;
    }
    
    convertSuperclasses(model);
    convertTemplates(model);
//    generateInlineClasses(model);
    addGetTypeMethod(model);
    for (auto &cls : model.classes) {
        cls->onLinked(model);
    }
}

void Linker::convertSuperclasses(Model &model) {
    for (auto &cls : model.classes) {
        if(!cls->parent_class_name.empty())
        {
            auto parent = model.get_class(cls->parent_class_name);
            assert(parent);
            if(!parent){
//                Error::exit(Error::UNKNOWN_SUPERCLASS, cls->name, superName);
            }
            cls->parent = parent;
            parent->subclasses.push_back(cls);
        }
    }
}

void Linker::convertTemplates(Model &model) {
//    for (auto &cls : model.classes) {
//        for (auto &member : cls->members) {
//            processTemplates(*member, model);
//        }
//    }
}

void Linker::processTemplates(Object &member, Model &model) {
//    std::vector<std::shared_ptr<Object>> args;
//    for (auto &arg : member.templateArgs) {
//        if (std::dynamic_pointer_cast<Object>(arg)) {
//            args.push_back(arg);
//        } else {
//            auto objType = getObjectType(model, arg->type);
//            processTemplates(*objType, model);
//            args.push_back(objType);
//        }
//    }
//    member.templateArgs = args;
}

void Linker::generateInlineFor(Class &cls) {
//    if (cls.superclasses.empty()) return;
//    for (auto &sup : cls.superclasses) {
//        if (!sup->isInline) continue;
//        for (auto &mem : sup->members) {
//            cls.members.push_back(std::make_shared<Object>(*mem));
//        }
//        for (auto &func : sup->functions) {
//            if (func->name == "operator ==" || func->name == "operator !=") continue;
//            cls.functions.push_back(std::make_shared<Function>(*func));
//        }
//        sup->subclasses.erase(std::remove(sup->subclasses.begin(), sup->subclasses.end(), std::make_shared<Class>(cls)), sup->subclasses.end());
//    }
//    cls.superclasses.erase(
//        std::remove_if(cls.superclasses.begin(), cls.superclasses.end(), [](auto &s){ return s->isInline; }),
//        cls.superclasses.end());
}

void Linker::addGetTypeMethod(Model &model) {
    for (auto &cls : model.classes) {
        if (cls->type == "class" && cls->auto_generated) {
            Object member;
            member.is_static = true;
            member.is_const = true;
            member.type = "string";
            member.name = "TYPE";
            member.value = "\"" + cls->name + "\"";
            member.access = AccessSpecifier::m_public;
            cls->members.push_back(std::move(member));

            Function method;
            method.name = "get_type";
            method.return_type = Objects::STRING;
            method.is_const = true;
            method.body += "return " + cls->name + "::TYPE;";
            method.is_virtual = cls->is_virtual || !cls->parent_class_name.empty() || !cls->subclasses.empty();
            cls->functions.push_back(std::move(method));
        }
    }
}
