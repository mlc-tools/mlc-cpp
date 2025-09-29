//
//  SavePluginPython.hpp
//

#pragma once

#include "../core/SavePluginBase.hpp"

class SavePluginPython : public SavePluginBase{
public:
    explicit SavePluginPython(Model& model);
    void save_files(bool combine_to_one=false);
};

