//
//  GeneratorFactoryPython.cpp
//

#include "GeneratorFactoryPython.hpp"
#include "../models/Model.hpp"
#include <sstream>

void GeneratorFactoryPython::generate(Model &model)
{
    std::ostringstream builders;
    const char *line =
        "\n        if type == \"{0}\":\n"
        "            from .{0} import {0}\n"
        "            return make_intrusive({0})";
    for (auto &cls : model.classes)
    {
        std::string name = cls->name;
        std::string l = line;
        size_t pos;
        while ((pos = l.find("{0}")) != std::string::npos)
            l.replace(pos, 3, name);
        builders << l;
    }
    std::string content;
    content += "# -*- coding: utf-8 -*-\n";
    content += "from .IntrusivePtr import make_intrusive\n\n";
    content += "class Factory(object):\n\n";
    content += "    @staticmethod\n";
    content += "    def build(type):\n";
    content += builders.str();
    content += "\n        return None\n";
    model.addFile(nullptr, "Factory.py", content);
}

