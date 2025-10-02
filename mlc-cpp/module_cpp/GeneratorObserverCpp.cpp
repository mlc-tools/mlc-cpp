//
//  GeneratorObserverCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 31.07.2025.
//

#include "GeneratorObserverCpp.hpp"
#include "Class.hpp"
#include "Common.hpp"
#include "Model.hpp"

void GeneratorObserverCpp::generate(Model &model) {
    auto cls = generate_class();

    model.add_class(cls);
}

std::shared_ptr<Class> GeneratorObserverCpp::generate_class() {
    auto cls = std::make_shared<Class>();
    cls->name = "Observable";
    cls->type = "class";
    cls->auto_generated = false;
    return cls;
}
