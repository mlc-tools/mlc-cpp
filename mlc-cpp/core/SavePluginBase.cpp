//
//  SavePluginBase.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//
#include "SavePluginBase.hpp"
#include "Model.hpp"    // defines Model, model.files and model.created_files
#include "Class.hpp"    // forward-declared in header
#include <algorithm>
#include "FileUtils.hpp"
#include "Error.hpp"
#include "Error.hpp"

SavePluginBase::SavePluginBase(Model &m)
  : model(m),
    files(m.files),
    created(m.created_files)
{}

SavePluginBase::~SavePluginBase() {
}

void SavePluginBase::save_files(bool combineToOne) {
    if (combineToOne) {
        sortFiles();
        saveOne();
    } else {
        saveAll();
    }
//    removeOldFiles();
}

std::pair<std::string,std::string>
SavePluginBase::createCombineFileHeader() {
    // Default: no header, no combined file
    return { "", "" };
}

std::string SavePluginBase::finalizeCombineFile(const std::string &content) {
    return content;
}

bool SavePluginBase::isNeedSaveFileOnCombine(const std::string &) {
    return false;
}

std::string SavePluginBase::removeIncludes(const std::string &c) {
    return c;
}

void SavePluginBase::saveOne() {
    sortFiles();

    auto [combine, combinePath] = createCombineFileHeader();
    for (auto &entry : files) {
        const auto &localPath = std::get<1>(entry);
        const auto &content   = std::get<2>(entry);

        combine = addToCombineFile(combine, content);
        if (isNeedSaveFileOnCombine(localPath)) {
            saveFile(localPath, finalizeCombineFile(combine));
        }
    }
    if (!combinePath.empty()) {
        saveFile(combinePath, finalizeCombineFile(combine));
    }
}

void SavePluginBase::saveAll() {
    // If out_dict is used, skip filesystem
    if (!model.out_dict.empty()) {
        for (auto &entry : files) {
            const auto &localPath = std::get<1>(entry);
            const auto &content   = std::get<2>(entry);
            model.out_dict[localPath] = content;
        }
        return;
    }

    for (auto &entry : files) {
        const auto &localPath = std::get<1>(entry);
        const auto &content   = std::get<2>(entry);
        saveFile(localPath, content);
    }
}

void SavePluginBase::saveFile(const std::string &localPath,
                              const std::string &content)
{
    created.push_back(localPath);
    std::string fullPath = FileUtils::normalizePath(model.out_directory) + localPath;
    bool existed = FileUtils::exists(fullPath);

    if (FileUtils::write(fullPath, content)) {
        Log::message(existed ? " Overwriting: " + localPath : " Create:      " + localPath);
    }
}

void SavePluginBase::sortFiles() {
    auto weight = [](const FileEntry &e){
        auto cls = std::get<0>(e);
        if (!cls) return std::string();
        std::string w = cls->name;
        // prepend '~' for each superclass level
        if (!cls->parent_class_name.empty()) {
            w = "~" + w;
        }
        return w;
    };
    std::sort(files.begin(), files.end(),
        [&](auto &a, auto &b){
            return weight(a) < weight(b);
        });
}

std::string SavePluginBase::addToCombineFile(
    const std::string &combine,
    const std::string &next)
{
    return combine + removeIncludes(next);
}

void SavePluginBase::removeOldFiles() {
    auto outDir = FileUtils::normalizePath(model.out_directory);
    for (auto &p : FileUtils::listFiles(outDir)) {
        if (std::find(created.begin(), created.end(), p) == created.end()
            && !p.ends_with(".pyc"))
        {
            FileUtils::remove(outDir + p);
            Log::debug(" Removed: " + p);
        }
    }
}
