//
//  FileUtils.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
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
    // Абсолютный нормализованный путь, даже если не существует на диске
    std::filesystem::path p(path);
    if(p.is_relative())
        p = std::filesystem::current_path() / p;

    p = p.lexically_normal();

    std::string s = p.string();
    char sep = std::filesystem::path::preferred_separator;
    if(s.empty() || s.back() != sep)
        s.push_back(sep);
    return s;
}

std::string getFilePath(const std::string& directory, const std::string& localPath){
    std::filesystem::path base(normalizePath(directory));
    std::filesystem::path rel(localPath);
    std::filesystem::path full = base / rel;
    full = full.lexically_normal();
    return full.string();
}

std::string directory_of_file(const std::string& path){
    return std::filesystem::path(path).parent_path();
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

bool write(const std::string &path, const std::string &content)
{
    if(path.find("Registrar.cpp") != std::string::npos)
    {
        std::cout << "";
    }
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
                        return false;
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
    std::fstream out(path, std::ios::binary | std::ios::out);
    if (!out.good()) {
        throw std::runtime_error("Cannot open file for writing: " + path);
    }
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    out.flush();

    return true;
}


Snapshot scan_dirs(const std::vector<std::string>& dirs, const std::vector<std::string>& exts)
{
    Snapshot s;
    for (const auto& d : dirs) {
        if (!std::filesystem::exists(d)) continue;
        for (const auto &entry : std::filesystem::recursive_directory_iterator(d)) {
            if (!entry.is_regular_file()) continue;
            if (!exts.empty()) {
                std::string ext = entry.path().extension().string();
                bool matched = false;
                for (const auto &fext : exts) {
                    if (ext == fext) { matched = true; break; }
                }
                if (!matched) continue;
            }
            const auto path = entry.path().string();
            std::error_code ec;
            auto ft = std::filesystem::last_write_time(entry.path(), ec);
            if (!ec) s.mtimes[path] = ft;
        }
    }
    return s;
}

bool has_changes(const Snapshot& oldS, const Snapshot& newS)
{
    if (oldS.mtimes.size() != newS.mtimes.size()) return true;
    // Check modified and added
    for (const auto& [p, t] : newS.mtimes) {
        auto it = oldS.mtimes.find(p);
        if (it == oldS.mtimes.end()) return true; // added
        if (it->second != t) return true; // modified
    }
    // Check removed
    for (const auto& [p, _] : oldS.mtimes) {
        if (!newS.mtimes.count(p)) return true; // removed
    }
    return false;
}

void diff_snapshots(const Snapshot& oldS, const Snapshot& newS, std::vector<std::string>& added_or_modified, std::vector<std::string>& removed)
{
    // added or modified
    for (const auto& [p, t] : newS.mtimes) {
        auto it = oldS.mtimes.find(p);
        if (it == oldS.mtimes.end() || it->second != t) {
            added_or_modified.push_back(p);
        }
    }
    // removed
    for (const auto& [p, _] : oldS.mtimes) {
        if (!newS.mtimes.count(p)) removed.push_back(p);
    }
}

}
