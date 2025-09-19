//
//  SavePluginCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once
#include "SavePluginBase.hpp"

class Model;

class SavePluginCpp : public SavePluginBase{
public:
    SavePluginCpp(Model& model);
    void save_files(bool combine_to_one=false);
};

