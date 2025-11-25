//
//  SavePluginCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once
#include "SavePluginBase.hpp"

class Model;
class SavePluginCpp : public SavePluginBase {
public:
    SavePluginCpp(Model &model, const FeatureUnityFile& feature_unity_file);
    void save_files(bool combine_to_one = false);
    
    virtual void saveOne() override;
    virtual void saveAll() override;
    virtual void saveByGroup() override;
};
