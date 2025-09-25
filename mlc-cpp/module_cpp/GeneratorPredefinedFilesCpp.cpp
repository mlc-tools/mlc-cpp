//
//  GeneratorPredefinedFilesCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorPredefinedFilesCpp.hpp"
#include "code/extensions.h"
#include "Model.hpp"
#include "WriterBase.hpp"
#include "Class.hpp"
#include <algorithm>
#include <sstream>
#include "Common.hpp"

std::string GeneratorPredefinedFiles::getNamespace() {
    return "mg";
}

void GeneratorPredefinedFiles::generate(Model &model) {
    // используем WriterBase для подготовки контента
    WriterBase writer;
    writer.set_model(model);

    // создаём config.h
    generateConfigFiles(model);

    // создаём базовый класс BaseEnum
    generateBaseEnumClass(model);

    // при наличии пользовательского генератора дополняем список файлов
//    if (model.custom_generator) {
//        auto extra = model.custom_generator->getPredefinedFiles(model);
//        FILES_DICT.insert(FILES_DICT.end(), extra.begin(), extra.end());
//    }

    // вставляем файлы из FILES_DICT
    for (auto &p : FILES_DICT) {
        std::string filename = p.first;
        // фильтрация по фичам
        if (filename.find("intrusive_ptr") != std::string::npos
            && !model.config.generate_intrusive) continue;
        if (filename.find("Factory") != std::string::npos
            && !model.config.generate_factory) continue;

        std::string content = p.second;
        // заместим namespace
        auto ns   = getNamespace();
        auto NS   = to_upper(ns);
        // actually uppercase:
        for (auto &c: NS) c = static_cast<char>(::toupper(c));
        // replace placeholders
        size_t pos;
        replace_all(content, "@{namespace}", ns);
        replace_all(content, "@{namespace_upper}", NS);

        // подготовка WriterBase (добавляет заголовки, форматирование и т.п.)
        content = writer.prepareFile(content);

        // заменим в имени файла
        while ((pos = filename.find("@{namespace}")) != std::string::npos)
            filename.replace(pos, 12, ns);

        // для Factory-файлов добавляем регистрацию, если нужно
        if (filename.find("Factory") != std::string::npos) {
            std::string reg = model.config.auto_registration
                ? FACTORY_REGISTRATION
                : std::string{};
            while ((pos = content.find("@{registration}")) != std::string::npos)
                content.replace(pos, 16, reg);
        }

        // сохраняем файл в модель
        model.addFile(nullptr, filename, content);
    }
}

void GeneratorPredefinedFiles::generateConfigFiles(Model &model) {
    std::vector<std::string> lines;
    for (auto &fmt : SerializeFormat_getAll()) {
        const std::string &name = SerializeFormat_to_str(fmt);
        bool supported = (model.config.serializeFormats & static_cast<int>(fmt)) != 0;
        lines.push_back( "#define SUPPORT_" + to_upper(name) + "_PROTOCOL " + (supported ? "true" : "false"));
    }
    
    const std::string pattern =
        "#ifndef __" + getNamespace() + "_Config_h__\n"
        "#define __" + getNamespace() + "_Config_h__\n\n"
        + join(lines, '\n') +
        "\n\n#endif //#ifndef __" + getNamespace() + "_Config_h__";

    model.addFile(nullptr, "config.h", pattern);
}

void GeneratorPredefinedFiles::generateBaseEnumClass(Model &model) {
    auto cls = std::make_shared<Class>();
    cls->name = "BaseEnum";
    cls->type = "class";
    cls->auto_generated = false;
    model.add_class(cls);
}

void GeneratorPredefinedFiles::generateObserverClass(Model &model) {
    auto cls = std::make_shared<Class>();
    cls->name = "Observer";
    cls->type = "class";
    cls->auto_generated = false;
    model.add_class(cls);
    
}
