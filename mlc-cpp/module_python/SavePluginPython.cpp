//
//  SavePluginPython.cpp
//

#include "SavePluginPython.hpp"

SavePluginPython::SavePluginPython(Model &model) : SavePluginBase(model) {}

void SavePluginPython::save_files(bool combine_to_one) {
    SavePluginBase::save_files(false);
}
