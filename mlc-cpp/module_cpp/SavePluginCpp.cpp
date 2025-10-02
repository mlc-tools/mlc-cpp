//
//  SavePluginCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "SavePluginCpp.hpp"

SavePluginCpp::SavePluginCpp(Model &model) : SavePluginBase(model) {}

void SavePluginCpp::save_files(bool combine_to_one) {
    SavePluginBase::save_files(false);
}
