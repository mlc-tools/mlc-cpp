//
//  GeneratorFactoryPython.cpp
//

#include "GeneratorFactoryPython.hpp"
#include "../models/Model.hpp"
#include "Common.hpp"
#include <sstream>

void GeneratorFactoryPython::generate(Model &model) {
    std::string builders;
    const char *line = R"(
        if type == "{0}":
            from .{0} import {0}
            return make_intrusive({0})
)";
    for (auto &cls : model.classes) {
        builders += format_indexes(line, cls->name);
    }
    std::string content;
    content += "# -*- coding: utf-8 -*-\n";
    content += "from .IntrusivePtr import make_intrusive\n\n";
    content += "class Factory(object):\n\n";
    content += "    @staticmethod\n";
    content += "    def build(type):\n";
    content += builders;
    content += "\n        return None\n";
    model.addFile(nullptr, "Factory.py", content);
}
