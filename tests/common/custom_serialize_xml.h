#pragma once

#include <variant>
#include <vector>
#include "pugixml/pugixml.hpp"
#include "ecs/Movement.h"
#include "ecs/Movement2.h"
#include "ecs/Movement3.h"
#include "ecs/Transform.h"

namespace mg {
namespace serializer_xml {

using ComponentVariant = std::variant<Transform, Movement, Movement2, Movement3>;

template <class T>
bool deserialize_variant_value(pugi::xml_node node, ComponentVariant &value,
                               const std::string &type) {
    if (type != T::TYPE)
        return false;

    value.emplace<T>();
    std::get<T>(value).deserialize_xml(node);
    return true;
}

inline void serialize(pugi::xml_node node, const ComponentVariant &value, const std::string &key) {
    pugi::xml_node child = key.empty() ? node : node.append_child(key.c_str());
    std::visit([&child](const auto &component) {
        child.append_attribute("type").set_value(component.get_type().c_str());
        component.serialize_xml(child);
    }, value);
}

inline void deserialize(pugi::xml_node node, ComponentVariant &value, const std::string &key) {
    pugi::xml_node child = key.empty() ? node : node.child(key.c_str());
    const std::string type = child.attribute("type").as_string();

    deserialize_variant_value<Transform>(child, value, type) ||
    deserialize_variant_value<Movement>(child, value, type) ||
    deserialize_variant_value<Movement2>(child, value, type) ||
    deserialize_variant_value<Movement3>(child, value, type);
}

inline void serialize(pugi::xml_node node, const std::vector<ComponentVariant> &value, const std::string &key) {
    pugi::xml_node child = key.empty() ? node : node.append_child(key.c_str());
    for (const auto &item : value) {
        serialize(child.append_child("item"), item, "");
    }
}

inline void deserialize(pugi::xml_node node, std::vector<ComponentVariant> &value, const std::string &key) {
    pugi::xml_node child = key.empty() ? node : node.child(key.c_str());
    value.clear();
    for (pugi::xml_node item : child.children("item")) {
        ComponentVariant component;
        deserialize(item, component, "");
        value.push_back(std::move(component));
    }
}

} // namespace serialize_xml
} // namespace mg
