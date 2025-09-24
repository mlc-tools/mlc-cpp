//
//  SavePluginBase.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include <fstream>

class Model;
class Class;
class SavePluginBase {
public:
    explicit SavePluginBase(Model &model);
    virtual ~SavePluginBase();

    /// Save all files collected in model.files.
    /// If combineToOne==true, may merge them into a single file.
    void save_files(bool combineToOne = true);
    void removeOldFiles();

protected:
    Model &model;
    std::vector<std::fstream*> streams;

    /// Override to provide a header for the combined file:
    ///   first = initial content, second = output path for combined file.
    virtual std::pair<std::string, std::string>
    createCombineFileHeader();

    /// Override to post-process combined content before writing.
    virtual std::string finalizeCombineFile(const std::string &content);

    /// Override to decide which files still need saving during combine pass.
    virtual bool isNeedSaveFileOnCombine(const std::string &localPath);

    /// Override to strip `#include`s or other headers when merging.
    virtual std::string removeIncludes(const std::string &fileContent);

private:
    using FileEntry = std::tuple<
        std::shared_ptr<Class>,  // may be null
        std::string,             // local path
        std::string              // content
    >;

    void saveOne();
    void saveAll();
    void saveFile(const std::string &localPath, const std::string &content);
    void sortFiles();
    std::string addToCombineFile(const std::string &current, const std::string &next);

    std::vector<FileEntry> &files;        // alias to model.files
    std::vector<std::string> &created;    // alias to model.created_files
};
