//
//  FileUtils.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace FileUtils {

std::string normalizePath(const std::string &path);

// Возвращает абсолютный нормализованный путь к файлу, составленный из каталога
// и локального пути. Каталог может не существовать — путь всё равно будет
// корректно нормализован.
std::string getFilePath(const std::string &directory,
                        const std::string &localPath);
std::string directory_of_file(const std::string &path);

std::vector<std::string> listFiles(const std::string &directory);
std::vector<std::string>
listFilesRecursive(const std::string &directory,
                   const std::vector<std::string> &extensions = {});

bool exists(const std::string &fullPath);
void remove(const std::string &fullPath);

std::string read(const std::string &path);
bool write(const std::string &fullPath, const std::string &content);

using FileTime = std::filesystem::file_time_type;

struct Snapshot {
    std::unordered_map<std::string, FileTime> mtimes;
};

Snapshot scan_dirs(const std::vector<std::string> &dirs,
                   const std::vector<std::string> &exts = {});

bool has_changes(const Snapshot &oldS, const Snapshot &newS);

void diff_snapshots(const Snapshot &oldS, const Snapshot &newS,
                    std::vector<std::string> &added_or_modified,
                    std::vector<std::string> &removed);

} // namespace FileUtils
