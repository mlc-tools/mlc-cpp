//
//  FileUtils.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace FileUtils {

    /**
     * @brief Recursively scans a directory and returns all file paths under it.
     *
     * @param directory The root directory to scan.
     * @return std::vector<std::string> A list of file paths (as strings) found in the directory and its subdirectories.
     * @throws std::filesystem::filesystem_error on access errors.
     */
    inline std::vector<std::string> listFilesRecursive(
            const std::string &directory,
            const std::vector<std::string> &extensions = {}) {
        std::vector<std::string> files;
        for (const auto &entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (!entry.is_regular_file())
                continue;
            if (!extensions.empty()) {
                std::string ext = entry.path().extension().string();
                bool matched = false;
                for (const auto &filterExt : extensions) {
                    if (ext == filterExt) {
                        matched = true;
                        break;
                    }
                }
                if (!matched)
                    continue;
            }
            files.push_back(entry.path().string());
        }
        return files;
    }

    /**
     * @brief Recursively scans a directory and returns all file paths under it as std::filesystem::path.
     *
     * @param directory The root directory to scan.
     * @return std::vector<std::filesystem::path> A list of file paths found in the directory and its subdirectories.
     * @throws std::filesystem::filesystem_error on access errors.
     */
    inline std::vector<std::filesystem::path> listFilesRecursivePath(const std::string &directory) {
        std::vector<std::filesystem::path> paths;
        for (const auto &entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                paths.push_back(entry.path());
            }
        }
        return paths;
    }

} // namespace FileUtils
