#pragma once

#include <variant>
#include <vector>
#include "jsoncpp/value.h"
#include "ecs/Movement.h"
#include "ecs/Movement2.h"
#include "ecs/Movement3.h"
#include "ecs/Transform.h"

namespace mg {
namespace serializer_json {

using ComponentVariant = std::variant<Transform, Movement, Movement2, Movement3>;

template <class T>
bool deserialize_variant_value(Json::Value &node, ComponentVariant &value, const std::string &type) {
    if (type != T::TYPE)
        return false;

    value.emplace<T>();
    std::get<T>(value).deserialize_json(node);
    return true;
}

inline void serialize(Json::Value &node, const ComponentVariant &value, const std::string &key) {
    Json::Value &child = key.empty() ? node : node[key];
    std::visit([&child](const auto &component) {
        child["type"] = component.get_type();
        component.serialize_json(child);
    },value);
}

inline void deserialize(Json::Value &node, ComponentVariant &value,
                        const std::string &key) {
    Json::Value &child = key.empty() ? node : node[key];
    const std::string type = child["type"].asString();
    deserialize_variant_value<Transform>(child, value, type) ||
    deserialize_variant_value<Movement>(child, value, type) ||
    deserialize_variant_value<Movement2>(child, value, type) ||
    deserialize_variant_value<Movement3>(child, value, type);
}

inline void serialize(Json::Value &node, const std::vector<ComponentVariant> &value,
                      const std::string &key) {
    Json::Value &child = key.empty() ? node : node[key];
    for (const auto &item : value) {
        Json::Value serialized_item(Json::objectValue);
        serialize(serialized_item, item, "");
        child.append(serialized_item);
    }
}

inline void deserialize(Json::Value &node, std::vector<ComponentVariant> &value,
                        const std::string &key) {
    Json::Value &child = key.empty() ? node : node[key];
    value.clear();
    for (Json::Value &item : child) {
        ComponentVariant component;
        deserialize(item, component, "");
        value.push_back(std::move(component));
    }
}

} // namespace serialize_json
} // namespace mg
