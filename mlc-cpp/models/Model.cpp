//
//  Model.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Model.hpp"
#include "../models/Object.hpp"
#include <cassert>

const std::unordered_set<std::string> Model::simpleTypes = {
    "int",
    "float",
    "bool",
    "string",
    "int64_t",
    "uint",
    "unsigned",
    "uint64_t",
    "double"
};

std::array<SerializeFormat, 2> SerializeFormat_getAll() {
    return std::array<SerializeFormat, 2>{
        SerializeFormat::Xml,
        SerializeFormat::Json,
    };
}

std::string SerializeFormat_to_str(SerializeFormat format){
    if(format == SerializeFormat::Xml) return "xml";
    if(format == SerializeFormat::Json) return "json";
    assert(0);
    return "";
}

void Model::create_std_types()
{
}

bool Model::is_simple_type(const std::string& type) const{
    return simpleTypes.count(type) > 0;
}

bool Model::is_skip(const Object& obj) {
    if(!obj.lang_specific.empty() && obj.lang_specific.count(language) == 0)
        return true;
    if(!is_side(obj.side))
        return true;
    return false;
}
