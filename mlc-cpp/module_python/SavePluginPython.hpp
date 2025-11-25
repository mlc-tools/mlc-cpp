//
//  SavePluginPython.hpp
//

#pragma once

#include "../core/SavePluginBase.hpp"

class SavePluginPython : public SavePluginBase {
public:
    explicit SavePluginPython(Model &model, const FeatureUnityFile& feature_unity_file);
    void save_files(bool combine_to_one = false);

    virtual std::pair<std::string, std::string> createCombineFileHeader() override;
    virtual std::string finalizeCombineFile(const std::string &content) override;
    virtual bool isNeedSaveFileOnCombine(const std::string &localPath) override;
    virtual std::string removeIncludes(const std::string &fileContent) override;
};
