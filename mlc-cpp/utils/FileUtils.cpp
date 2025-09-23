//
//  FileUtils.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstring>
#include "FileUtils.hpp"


namespace FileUtils {

std::vector<std::string> listFilesRecursive(const std::string &directory, const std::vector<std::string> &extensions) {
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

std::string normalizePath(const std::string& path){
    //TODO:
    return path;
}

std::vector<std::string> listFiles(const std::string &directory){
    std::vector<std::string> files;
    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        if (!entry.is_regular_file())
            continue;
        files.push_back(entry.path().string());
    }
    return files;
}

bool exists(const std::string& fullPath){
    return std::filesystem::exists(fullPath);
}

void remove(const std::string& fullPath){
    std::filesystem::remove(fullPath);
}

std::string read(const std::string &path) {
    std::fstream in(path, std::ios::in);
    if (!in) {
        throw std::runtime_error("Cannot open file for reading: " + path);
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::pair<bool, std::fstream*> write(const std::string &path, const std::string &content)
{
    // Fast path: if file exists and size matches, compare bytes; otherwise skip read.
    if (std::filesystem::exists(path)) {
        try {
            auto sz = std::filesystem::file_size(path);
            if (sz == content.size()) {
                std::ifstream in(path, std::ios::binary);
                if (in) {
                    std::string buf;
                    buf.resize(static_cast<size_t>(sz));
                    in.read(&buf[0], static_cast<std::streamsize>(sz));
                    if (in.good() && std::memcmp(buf.data(), content.data(), buf.size()) == 0) {
                        return {false, nullptr};
                    }
                }
            }
        } catch (...) {
            // If file_size or read fails, proceed to overwrite
        }
    }

    // Ensure parent directory exists
    std::filesystem::path p(path);
    if (auto parent = p.parent_path(); !parent.empty()) {
        std::filesystem::create_directories(parent);
    }

    // Open stream for writing (truncate)
    auto *out = new std::fstream(path, std::ios::binary | std::ios::out);
    if (!out->good()) {
        delete out;
        throw std::runtime_error("Cannot open file for writing: " + path);
    }
    out->write(content.data(), static_cast<std::streamsize>(content.size()));
    out->flush();

    return {true, out};
}

}
