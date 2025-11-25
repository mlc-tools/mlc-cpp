//
//  SavePluginPython.cpp
//

#include "SavePluginPython.hpp"
#include <re2/re2.h>
#include "utils/Common.hpp"

SavePluginPython::SavePluginPython(Model &model, const FeatureUnityFile& feature_unity_file) : SavePluginBase(model, feature_unity_file) {}

void SavePluginPython::save_files() {
    SavePluginBase::save_files();
}

std::pair<std::string, std::string> SavePluginPython::createCombineFileHeader(){
    auto header = R"(# -*- coding: utf-8 -*-
import json
import xml.etree.ElementTree as ET
)";
    return {header, "mg.py"};
}
std::string SavePluginPython::finalizeCombineFile(const std::string &content){
    return SavePluginBase::finalizeCombineFile(content);
}
bool SavePluginPython::isNeedSaveFileOnCombine(const std::string &localPath){
    return localPath == "__init__.py";
}
std::string SavePluginPython::removeIncludes(const std::string &fileContent){
    auto content = fileContent;
    RE2::GlobalReplace(&content, R"(\n\s*from.+import.+)", "");
    RE2::GlobalReplace(&content, R"(\n\s*import.+)", "");
    replace_all(content, "# -*- coding: utf-8 -*-\n", "");
    return content;
}
