//
//  SavePluginBase.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class Model;
class Class;
class FeatureUnityFile;

class SavePluginBase {
public:
    explicit SavePluginBase(Model &model, const FeatureUnityFile& feature_unity_file);
    virtual ~SavePluginBase();

    /// Save all files collected in model.files.
    /// If combineToOne==true, may merge them into a single file.
    void save_files(bool combineToOne = true);
    void removeOldFiles();

protected:
    Model &model;
    std::vector<std::fstream *> streams;
    virtual std::pair<std::string, std::string> createCombineFileHeader();
    virtual std::string finalizeCombineFile(const std::string &content);
    virtual bool isNeedSaveFileOnCombine(const std::string &localPath);
    virtual std::string removeIncludes(const std::string &fileContent);

protected:
    using FileEntry = std::tuple<std::shared_ptr<Class>, // may be null
                                 std::string,            // local path
                                 std::string             // content
                                 >;

    
    void saveOne();
    void saveAll();
    void saveByGroup();
    void saveFile(const std::string &localPath, const std::string &content);
    void sortFiles();
    std::string addToCombineFile(const std::string &current,
                                 const std::string &next);

    std::vector<FileEntry> &files;     // alias to model.files
    std::vector<std::string> &created; // alias to model.created_files
    const FeatureUnityFile& _feature_unity_file;
};
