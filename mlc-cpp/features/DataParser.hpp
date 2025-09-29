//
//  DataParser.hpp
//  mlc-cpp
//
//  Parses XML/JSON data files into a common structure
//  and flushes merged data to an output file (data.xml/json).
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "pugixml/pugixml.hpp"
#include "jsoncpp/json.h"

#include "Class.hpp"

namespace pugi { class xml_document; class xml_node; }
namespace Json { class Value; }

class Model;

class DataParser {
public:
    DataParser(Model& model,
               const std::vector<std::shared_ptr<Class>>& classes,
               std::function<bool(const std::string&)> filter_func);

    void parse(const std::vector<std::string>& directories);
    void flush(const std::string& out_data_directory);

private:
    struct XmlItem {
        std::string key;
        std::unique_ptr<pugi::xml_document> doc; // holds single root element ('value')
    };

    void parseDirectory(const std::string& directory);
    void parseXml(const std::string& full_path);
    void parseJson(const std::string& full_path);
    void parseJsonNode(const std::string& full_path, const std::string& key, const Json::Value& dict);

    void validate();
    void validateType(const std::string& type, const std::string& filename);

    std::string flushXml() const;
    std::string flushJson() const;

private:
    // State
    Model& _model;
    std::unordered_map<std::string, std::vector<XmlItem>> xmlObjects_;
    std::unordered_map<std::string, std::vector<Json::Value>> jsonObjects_;
    std::vector<std::shared_ptr<Class>> classes_;
    std::function<bool(const std::string&)> filter_;
    std::vector<std::string> directories_;
};

