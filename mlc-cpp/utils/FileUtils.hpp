//
//  FileUtils.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//
#pragma once

#include <string>
#include <vector>

namespace FileUtils {

/**
 * @brief Recursively scans a directory and returns all file paths under it.
 *
 * @param directory The root directory to scan.
 * @return std::vector<std::string> A list of file paths (as strings) found in the directory and its subdirectories.
 * @throws std::filesystem::filesystem_error on access errors.
 */
std::vector<std::string> listFilesRecursive(const std::string &directory, const std::vector<std::string> &extensions = {});

std::string normalizePath(const std::string& path);

std::vector<std::string> listFiles(const std::string &directory);
bool exists(const std::string& fullPath);
void remove(const std::string& fullPath);

std::string read(const std::string &path);
std::pair<bool, std::fstream*> write(const std::string &fullPath, const std::string &content);


} // namespace FileUtils
