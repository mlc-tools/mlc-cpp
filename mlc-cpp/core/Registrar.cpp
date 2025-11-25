//
//  Registrar.cpp
//  mlc-cpp
//

#include "Registrar.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "Class.hpp"
#include "Common.hpp"
#include "Model.hpp"

namespace {
static std::string build_hpp(const std::string &ns) {
    return format_indexes(R"(#ifndef __Registrar_h__
#define __Registrar_h__

namespace {0}
{
    void register_classes();
}

#endif
)",
                          ns);
}

static std::string build_cpp(const std::string &ns,
                             const std::string &factory_header,
                             const std::string &includes,
                             const std::string &registrations) {
    return format_indexes(R"(#include "Registrar.h"
#include "{1}"
{2}
void {0}::register_classes()
{
{3}
}
)",
                          ns, factory_header, includes, registrations);
}
} // namespace

std::shared_ptr<Class> Registrar::get_mock() {
    auto cls = std::make_shared<Class>();
    cls->name = "Registrar";
    cls->type = "class";
    cls->auto_generated = false;
    return cls;
}

std::string
Registrar::get_include_path_to_class(const std::shared_ptr<Class> &from,
                                     const std::shared_ptr<Class> &to) {
    // Registrar lives at project root (mock has empty group),
    // so include either "Group/Class.h" or "Class.h"
    (void)from; // from is not used since mock is always root-level
    std::string path = "\"";
    if (!to->group.empty()) {
        path += to->group + "/";
    }
    path += to->name + ".h\"";
    return path;
}

void Registrar::generate(Model &model) {
    // Mirror Python: skip when auto-registration is enabled
    if (model.config.auto_registration)
        return;
    if(model.config.language != "cpp"){
        return;
    }

    auto mock = Registrar::get_mock();
    model.add_class(mock);

    std::string includes;
    std::string registrations;

    for (auto &cls : model.classes) {
        if (cls->name == "Temp")
            std::cout << "";
        if (cls->is_abstract || cls->has_abstract_method())
            continue;
        if (!cls->has_method("get_type"))
            continue;
        if (model.is_skip(*cls.get()))
            continue;
        includes += "#include " + get_include_path_to_class(mock, cls) + "\n";
        registrations += "    Factory::shared().registrationCommand<" +
                         cls->name + ">(" + cls->name + "::TYPE);\n";
    }

    // Files
    const std::string ns = model.config.namespace_name.empty()
                               ? std::string("mg")
                               : model.config.namespace_name;
    const std::string factory_header = ns + "_Factory.h";
    const std::string hpp = build_hpp(ns);
    const std::string cpp =
        build_cpp(ns, factory_header, includes, registrations);

    model.addFile(nullptr, "Registrar.h", hpp);
    model.addFile(nullptr, "Registrar.cpp", cpp);
}
