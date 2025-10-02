//
//  GeneratorPackagePython.cpp
//

#include "GeneratorPackagePython.hpp"
#include "../models/Model.hpp"
#include "../models/Serialize.hpp"

void GeneratorPackagePython::generate(Model &model) {
    auto mask = model.config.serializeFormats;
    bool xml = (mask & static_cast<int>(SerializeFormat::Xml)) != 0;
    bool json = (mask & static_cast<int>(SerializeFormat::Json)) != 0;

    std::string content;
    content += "\nclass Config:";
    content += "\n\n    def __init__(self):";
    if (xml)
        content += "\n        SUPPORT_XML_PROTOCOL = True";
    else
        content += "\n        SUPPORT_XML_PROTOCOL = False";
    if (json)
        content += "\n        SUPPORT_JSON_PROTOCOL = True";
    else
        content += "\n        SUPPORT_JSON_PROTOCOL = False";
    content += "\n";

    model.addFile(nullptr, "__init__.py", content);
}
