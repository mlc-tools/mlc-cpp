//
//  Config.cpp
//  mlc-cpp
//
//  JSON loader for Config and helper builders
//

#include "Config.hpp"
#include "FileUtils.hpp"

#include "jsoncpp/json.h"
#include <re2/re2.h>
#include <iostream>

void loadFeatures(std::vector<FeatureVariant>& features, const Json::Value& root);

std::function<bool(const std::string &)>
make_re2_filter_from_patterns(const std::vector<std::string> &raws) {
    if (raws.empty())
        return {};
    auto inc = std::make_shared<std::vector<std::unique_ptr<RE2>>>();
    auto exc = std::make_shared<std::vector<std::unique_ptr<RE2>>>();
    for (const auto &p : raws) {
        if (!p.empty() && p[0] == '!')
            exc->emplace_back(std::make_unique<RE2>(p.substr(1)));
        else
            inc->emplace_back(std::make_unique<RE2>(p));
    }
    return [inc, exc](const std::string &path) {
        for (const auto &re : *exc)
            if (RE2::PartialMatch(path, *re))
                return false;
        if (inc->empty())
            return true;
        for (const auto &re : *inc)
            if (RE2::PartialMatch(path, *re))
                return true;
        return false;
    };
}

std::vector<std::string>
split_comma_allow_lists(const std::vector<std::string> &vals) {
    std::vector<std::string> out;
    out.reserve(vals.size());
    for (auto &v : vals) {
        size_t start = 0, pos = 0;
        while ((pos = v.find(',', start)) != std::string::npos) {
            auto tok = v.substr(start, pos - start);
            if (!tok.empty())
                out.push_back(tok);
            start = pos + 1;
        }
        auto last = v.substr(start);
        if (!last.empty())
            out.push_back(last);
    }
    return out;
}

std::vector<std::string> read_patterns(const Json::Value &v) {
    std::vector<std::string> pats;
    if (v.isString())
        pats.push_back(v.asString());
    else if (v.isArray()) {
        for (const auto &it : v)
            if (it.isString())
                pats.push_back(it.asString());
    }
    return split_comma_allow_lists(pats);
}

void load_dirs(const Json::Value &v, std::vector<std::string> &out) {
    if (v.isString())
        out.push_back(v.asString());
    else if (v.isArray()) {
        for (const auto &it : v)
            if (it.isString())
                out.push_back(it.asString());
    }
}

Config Config::loadFile(const std::string &path, std::string &err) {
    auto content = FileUtils::read(path);
    if (content.empty()) {
        err = "Config is empty or not found: " + path;
        return Config();
    }
    return loadString(content, err);
}

Config Config::loadString(const std::string &content, std::string &err) {
    Config config;
    Json::CharReaderBuilder b;
    std::string errs;
    std::unique_ptr<Json::CharReader> rdr(b.newCharReader());
    Json::Value root;
    if (!rdr->parse(content.data(), content.data() + content.size(), &root,
                    &errs)) {
        err = errs;
        return config;
    }
    
    // Gather global sources
    std::vector<std::string> src_cfgs, src_data;
    if (root.isMember("source")) {
        const auto &src = root["source"];
        if (src.isMember("configs"))
            load_dirs(src["configs"], src_cfgs);
        if (src.isMember("data"))
            load_dirs(src["data"], src_data);
    }
    
    const auto &gens = root["generation"];
    if (!gens.isArray() || gens.empty()) {
        err = "Config: 'generation' array is missing or empty";
        return config;
    }
    
    if(root.isMember("features")){
        std::cerr << "features in root not supported more" << std::endl;
        exit(1);
    }
    
    for (const auto &g : gens) {
        config.configs_directories = src_cfgs;
        config.data_directories = src_data;
        
        Job job;
        if (g.isMember("out"))
            job.out_directory = g["out"].asString();
        if (g.isMember("data_out"))
            job.out_data_directory = g["data_out"].asString();
        if (g.isMember("language"))
            job.language = g["language"].asString();
        if (g.isMember("namespace_name"))
            job.namespace_name = g["namespace_name"].asString();
        if (g.isMember("only_data"))
            job.only_data = g["only_data"].asBool();
        if (g.isMember("php_validate"))
            job.php_validate = g["php_validate"].asBool();
        if (g.isMember("allow_different_virtual"))
            job.allow_different_virtual = g["allow_different_virtual"].asBool();
        if (g.isMember("test_script"))
            job.test_script = g["test_script"].asString();
        if (g.isMember("test_script_args"))
            job.test_script_args = g["test_script_args"].asString();
        
        if (g.isMember("side") && g["side"].isString()) {
            std::string s = g["side"].asString();
            for (char &ch : s)
                ch = static_cast<char>(::tolower(static_cast<unsigned char>(ch)));
            if (s == "client")
                job.side = Side::client;
            else if (s == "server")
                job.side = Side::server;
            else
                job.side = Side::both;
        }
        
        if (g.isMember("generate_tests"))
            job.generate_tests = g["generate_tests"].asBool();
        if (g.isMember("generate_intrusive"))
            job.generate_intrusive = g["generate_intrusive"].asBool();
        if (g.isMember("generate_factory"))
            job.generate_factory = g["generate_factory"].asBool();
        if (g.isMember("auto_registration"))
            job.auto_registration = g["auto_registration"].asBool();
        if (g.isMember("generate_ref_counter"))
            job.generate_ref_counter = g["generate_ref_counter"].asBool();
        if (g.isMember("user_includes"))
            job.user_includes = g["user_includes"].asBool();
        if (g.isMember("empty_methods"))
            job.empty_methods = g["empty_methods"].asBool();
        
        if (g.isMember("serialize_protocol")) {
            std::vector<std::string> tokens;
            const auto &sp = g["serialize_protocol"];
            if (sp.isString())
                tokens.push_back(sp.asString());
            else if (sp.isArray()) {
                for (const auto &it : sp)
                    if (it.isString())
                        tokens.push_back(it.asString());
            }
            tokens = split_comma_allow_lists(tokens);
            if (!tokens.empty()) {
                int mask = 0;
                std::vector<SerializeFormat> protos;
                for (auto &t : tokens) {
                    std::string s = t;
                    for (char &ch : s)
                        ch = static_cast<char>(
                                               ::tolower(static_cast<unsigned char>(ch)));
                    if (s == "xml") {
                        mask |= static_cast<int>(SerializeFormat::Xml);
                        protos.push_back(SerializeFormat::Xml);
                    } else if (s == "json") {
                        mask |= static_cast<int>(SerializeFormat::Json);
                        protos.push_back(SerializeFormat::Json);
                    }
                }
                if (mask != 0) {
                    job.serializeFormats = mask;
                    job.serialize_protocol = std::move(protos);
                }
            }
        }
        
        if (g.isMember("filter_code")) {
            auto pats = read_patterns(g["filter_code"]);
            if (!pats.empty())
                job.filter_code = make_re2_filter_from_patterns(pats);
        }
        if (g.isMember("filter_data")) {
            auto pats = read_patterns(g["filter_data"]);
            if (!pats.empty())
                job.filter_data = make_re2_filter_from_patterns(pats);
        }
        
        loadFeatures(job.features, g["features"]);
        config.jobs.push_back(std::move(job));
    }
    
    if(root.isMember("common_features")){
        std::vector<FeatureVariant> common_features;
        loadFeatures(common_features, root["common_features"]);
        for(auto& job : config.jobs){
            job.features.insert(job.features.end(), common_features.begin(), common_features.end());
        }
    }
    
    config.initialize();
    return config;
}
    
void loadFeatures(std::vector<FeatureVariant>& vfeatures, const Json::Value& features){
    const auto &feature_unit_tests = features["unit_tests"];
    if (feature_unit_tests.isObject()) {
        FeatureUnitTests feature;
        vfeatures.push_back(std::move(feature));
    }
    const auto &feature_visitor = features["visitor"];
    if (feature_visitor.isObject()) {
        FeatureVisitor vis;
        vfeatures.push_back(std::move(vis));
    }
    const auto &feature_ecs = features["ecs"];
    if (feature_ecs.isObject()) {
        FeatureEcs ecs;
        ecs.model_base = feature_ecs["model_base"].asString();
        ecs.component_base = feature_ecs["component_base"].asString();
        vfeatures.push_back(std::move(ecs));
    }

    const auto &feature_data_storage = features["data_storage"];
    if (feature_data_storage.isObject()) {
        FeatureDataStorage f;
        if(feature_data_storage.isMember("private_members"))
            f.private_members = feature_data_storage["private_members"].asBool();
        vfeatures.push_back(std::move(f));
    }

    const auto &feature_ref_counter = features["ref_counter"];
    if (feature_ref_counter.isObject()) {
        FeatureRefCounter f;
        vfeatures.push_back(std::move(f));
    }
    
    const auto &feature_operator_equals = features["operator_equals"];
    if (feature_operator_equals.isObject()) {
        FeatureOperatorEquals f;
        vfeatures.push_back(std::move(f));
    }
    
    const auto &feature_bindings = features["bindings"];
    if (feature_bindings.isObject()) {
        FeatureBindings f;
        for(auto getter : feature_bindings["getters"]){
            FeatureBindings::Getter g;
            g.starts_with = getter["starts_with"].asString();
            g.equals = getter["equals"].asString();
            g.value = getter["value"].asString();
            f.getters.push_back(std::move(g));
        }
        vfeatures.push_back(std::move(f));
    }
    const auto &feature_unity = features["unity_file"];
    if (feature_unity.isObject()) {
        FeatureUnityFile f;
        f.all_to_one = feature_unity["all_to_one"].asBool();
        f.group_to_one = feature_unity["group_to_one"].asBool();
        vfeatures.push_back(std::move(f));
    }
}

bool FeatureBindings::Getter::is_right(const std::string& class_name) const{
    if(!starts_with.empty() && class_name.find(starts_with) == 0)
        return true;
    return equals == class_name;
}
std::string FeatureBindings::get_getter(const std::string& class_name) const{
    for(auto& getter : getters){
        if(getter.is_right(class_name))
            return getter.value;
    }
    return "";
}
