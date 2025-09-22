//
//  DataParser.cpp
//  mlc-cpp
//

#include "DataParser.hpp"

#include <sstream>
#include <regex>

#include "GeneratorDataStorageBase.hpp" // getDataName/getDataListName/getClassNameFromDataName
#include "FileUtils.hpp"
#include "Error.hpp"

#include "pugixml/pugixml.hpp"
#include "jsoncpp/json.h"
#include "Model.hpp"

DataParser::DataParser(Model& model,
                       const std::vector<std::shared_ptr<Class>>& classes,
                       std::function<bool(const std::string&)> filter_func)
: _model(model)
,classes_(classes)
, filter_(std::move(filter_func))
{
}

void DataParser::parse(const std::vector<std::string>& directories) {
    directories_.reserve(directories.size());
    for (auto &dir : directories) {
        directories_.push_back(FileUtils::normalizePath(dir));
    }
    for (auto &dir : directories_) {
        parseDirectory(dir);
    }
    validate();
}

void DataParser::flush(const std::string& out_data_directory) {

    std::string buffer;
    std::string fmtName;
    if (_model.serializeFormats & static_cast<int>(SerializeFormat::Xml)) {
        buffer = flushXml();
        fmtName = "xml";
    } else {
        buffer = flushJson();
        fmtName = "json";
    }

    std::string filename = "data." + fmtName;
    std::string outDir = FileUtils::normalizePath(out_data_directory);
    std::string fullPath = outDir + filename;
    bool existed = FileUtils::exists(fullPath);
    auto [ok, _] = FileUtils::write(fullPath, buffer);
    if (ok) {
        Log::message(std::string(existed ? " Overwriting: " : " Create: ") + filename);
    }
}

void DataParser::parseDirectory(const std::string& directory) {
    auto files = FileUtils::listFiles(directory);
    for (auto &full_path : files) {
        if (filter_ && !filter_(full_path)) continue;
        if (_model.serializeFormats & static_cast<int>(SerializeFormat::Xml) && full_path.size() >= 4 && full_path.rfind(".xml") == full_path.size()-4) {
            parseXml(full_path);
        } else if (_model.serializeFormats & static_cast<int>(SerializeFormat::Json) && full_path.size() >= 5 && full_path.rfind(".json") == full_path.size()-5) {
            parseJson(full_path);
        }
    }
}

void DataParser::parseXml(const std::string& full_path) {
    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load_file(full_path.c_str());
    if (!res) {
        Error::exit(Error::CANNOT_PARSE_XML, full_path);
        return;
    }
    pugi::xml_node root = doc.document_element();

    auto addNode = [&](pugi::xml_node obj){
        std::string tag = obj.name();
        validateType(tag, full_path);
        XmlItem item;
        item.key = obj.attribute("name").as_string();
        item.doc = std::make_unique<pugi::xml_document>();
        // copy as 'value'
        pugi::xml_node copied = item.doc->append_copy(obj);
        copied.set_name("value");
        xmlObjects_[tag].push_back(std::move(item));
    };

    if (std::string(root.name()) == "data") {
        for (pugi::xml_node node : root.children()) {
            addNode(node);
        }
    } else {
        addNode(root);
    }
}

void DataParser::parseJson(const std::string& full_path) {
    std::string content = FileUtils::read(full_path);
    Json::Value root;
    Json::Reader reader;
    reader.parse(content, root);

    if (root.isObject()) {
        for (const auto &name : root.getMemberNames()) {
            parseJsonNode(full_path, name, root);
        }
    } else if (root.isArray()) {
        for (const auto &dict : root) {
            if (!dict.isObject()) continue;
            for (const auto &name : dict.getMemberNames()) {
                parseJsonNode(full_path, name, dict);
            }
        }
    }
}

void DataParser::parseJsonNode(const std::string& full_path,
                               const std::string& key,
                               const Json::Value& dict)
{
    validateType(key, full_path);
    jsonObjects_[key].push_back(dict);
}

void DataParser::validate() {
    // Placeholder — Python version is pass
}

void DataParser::validateType(const std::string& type, const std::string& filename) {
    std::string class_name = getClassNameFromDataName(type);
    bool valid = false;
    for (auto &cls : classes_) {
        if (cls->name == class_name) {
            valid = cls->is_storage;
            break;
        }
    }
    if (!valid) {
        Error::exit(Error::UNKNOWN_DATA_TYPE, type, class_name, filename);
    }
}

std::string DataParser::flushXml() const {
    pugi::xml_document out;
    pugi::xml_node root = out.append_child("data");

    for (const auto &kv : xmlObjects_) {
        const std::string &type = kv.first;
        const auto &vec = kv.second;
        std::string name = getDataListName(getDataName(type));
        pugi::xml_node list = root.append_child(name.c_str());
        for (const auto &item : vec) {
            pugi::xml_node pair = list.append_child("pair");
            pair.append_attribute("key").set_value(item.key.c_str());
            // append 'value' node (copy from item's doc)
            if (item.doc && item.doc->first_child())
                pair.append_copy(item.doc->first_child());
        }
    }
    std::ostringstream ss;
    out.save(ss, "");
    std::string text = ss.str();
    return text;
}

std::string DataParser::flushJson() const {
    Json::Value dict(Json::objectValue);
    for (const auto &kv : jsonObjects_) {
        const std::string &key = kv.first;
        const auto &vec = kv.second;
        std::string name = getDataListName(getDataName(key));
        Json::Value arr(Json::arrayValue);
        for (const auto &object_ : vec) {
            if (!object_.isObject()) continue;
            for (const auto &member : object_.getMemberNames()) {
                const Json::Value &val = object_[member];
                Json::Value pair(Json::objectValue);
                pair["key"] = val.isObject() && val.isMember("name") ? val["name"] : Json::Value("");
                pair["value"] = val;
                arr.append(pair);
            }
        }
        dict[name] = arr;
    }
    Json::StreamWriterBuilder w;
    w["indentation"] = "  ";
    return Json::writeString(w, dict);
}
