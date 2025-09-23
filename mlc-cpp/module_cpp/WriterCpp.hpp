//
//  WriterCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <array>
#include <memory>

#include "WriterBase.hpp"
#include "Class.hpp"
#include "Function.hpp"
#include "Object.hpp"
#include "RegexPatternCpp.hpp"

class WriterCpp : public WriterBase {
public:
    explicit WriterCpp();

    std::vector<std::pair<std::string,std::string>>
    writeClass(const std::shared_ptr<Class> &cls) override;

protected:
    // Cache for member declarations and inits: name → {decl, init, staticInit}
    std::unordered_map<std::string, std::array<std::string,3>> objectsCache_;
    // Cache for methods: ptr(Function) → {hpp, cpp}
    std::unordered_map<const Function*, std::pair<std::string,std::string>> methodsCache_;
    // Only HPP snippets for templated methods
    std::vector<std::string> methodsCacheWithTemplates_;

    // Step-by-step overrides
    std::tuple<std::string,std::string,std::string> writeObject(const Object &obj) override;
    std::pair<std::string,std::string> writeFunction(const Function &fn) override;

    std::tuple<std::string,std::set<std::string>,std::set<std::string>,std::set<std::string>>
    writeHpp(const std::shared_ptr<Class> &cls);

    std::string writeCpp(const std::shared_ptr<Class> &cls,
                         const std::set<std::string> &includes,
                         const std::set<std::string> &fwdDecl,
                         const std::set<std::string> &fwdDeclOut);

private:
    // Helpers
    std::string writeFunctionHpp(const Function &method);
    std::string writeFunctionCpp(const Function &method);
    std::string createFunctionHppArgs(const Function &method);
    std::string createFunctionCppArgs(const Function &method);
    bool isOverride(const Function &method, const std::shared_ptr<Class> &cls);

    // Member code generators
    std::string writeMemberDeclaration(const Object &obj);
    std::string writeMemberEnumDeclaration(const Object &obj);
    std::string writeMemberStaticInit(const Class &cls, const Object &obj);
    std::string writeMemberInitialization(const Object &obj);

    // Named-object helper
    std::string writeNamedObject(const Object &obj,
                                        const std::string &name,
                                        bool tryConstRef,
                                        bool useIntrusive);

    std::string convertType(const std::string &t);

    // Include resolution
    std::tuple<std::set<std::string>,std::set<std::string>,std::set<std::string>>
    getIncludesForHeader(const std::shared_ptr<Class> &cls);

    std::set<std::string> getIncludesForMethod(const std::shared_ptr<Class> &cls,
                                               const std::string &functionsText,
                                               const std::set<std::string> &hppIncludes);

    std::string getIncludesForSource(const std::shared_ptr<Class> &cls,
                                     const std::string &functionsText,
                                     std::set<std::string> hppIncludes,
                                     const std::set<std::string> &fwdDecl,
                                     const std::set<std::string> &fwdDeclOut);

    std::string buildIncludes(const std::shared_ptr<Class> &cls,
                              const std::set<std::string> &includes);

    std::string buildForwardDeclarations(const std::set<std::string> &decls);

    // Filename helpers
    std::string getFilename(const std::shared_ptr<Class> &cls, const std::string &ext);
    std::string getPathToRoot(const std::shared_ptr<Class> &cls);
    
    std::string getTemplates(const Object &obj);

    // Source templates
    static constexpr const char *HEADER = R"(#ifndef __{namespace}_{class_name}_h__
#define __{namespace}_{class_name}_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
{includes}

{forward_declarations_out}
namespace {namespace}
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
{forward_declarations}

    class {class_name}{superclass}
    {
    public:
        {class_name}({constructor_args});
        {destructor}
{functions}
{members}
    };
} //namespace {namespace}

#endif //#ifndef __{namespace}_{class_name}_h__
)";
    static constexpr const char *SOURCE = R"(#include "intrusive_ptr.h"
#include "{path_to_root}{namespace}_Factory.h"
{includes}
#include "{path_to_root}{namespace}_extensions.h"
#include "{path_to_root}SerializerJson.h"
#include "{path_to_root}SerializerXml.h"

namespace {namespace}
{
    {static_initializations}
    {registration}
    {class_name}::{class_name}({constructor_args})
    {initializations}{
    {constructor_body}
    }

    {destructor}

{functions}} //namespace {namespace}
)";
};
