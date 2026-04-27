//
//  serializer_json.h
//  mlc
//
//  Created by  vladimirtolmachev on 27.04.2026.
//

#pragma mark SERIALIZER_JSON_HPP
const std::string SERIALIZER_JSON_HPP = R"__EXT__(#ifndef __mg_serializer_json_H__
#define __mg_serializer_json_H__

#include <string>
#include <map>
#include <vector>
#include "intrusive_ptr.h"
#include "SerializerCommon.h"
#include "DataStorage.h"
#include "mg_Factory.h"

namespace Json
{
    class Value;
    class ValueIterator;
}

namespace mg
{
namespace serializer_json
{

    Json::Value& add_child(Json::Value& node, const std::string &name);
    Json::Value& add_array(Json::Value& node, const std::string &name);
    Json::Value& add_array_item(Json::Value& node);

    void add_attribute(Json::Value& node, const std::string &key, const int &value, int default_value = 0);
    void add_attribute(Json::Value& node, const std::string &key, const int64_t &value, int64_t default_value = 0);
    void add_attribute(Json::Value& node, const std::string &key, const unsigned int &value, unsigned int default_value = 0);
    void add_attribute(Json::Value& node, const std::string &key, const uint64_t &value, uint64_t default_value = 0);
    void add_attribute(Json::Value& node, const std::string &key, const bool &value, bool default_value = false);
    void add_attribute(Json::Value& node, const std::string &key, const float &value, float default_value = 0.f);
    void add_attribute(Json::Value& node, const std::string &key, const double &value, double default_value = 0.f);
    void add_attribute(Json::Value& node, const std::string &key, const std::string &value, const std::string &default_value);

    void add_array_item(Json::Value& node, const int &value);
    void add_array_item(Json::Value& node, const int64_t &value);
    void add_array_item(Json::Value& node, const unsigned int &value);
    void add_array_item(Json::Value& node, const uint64_t &value);
    void add_array_item(Json::Value& node, const bool &value);
    void add_array_item(Json::Value& node, const float &value);
    void add_array_item(Json::Value& node, const double &value);
    void add_array_item(Json::Value& node, const std::string &value);

    template <class T>
    typename std::enable_if<is_enum<T>::value, void>::type
    add_array_item(Json::Value& node, const T& value)
    {
        add_array_item(node, value.str());
    }

    template<class T>
    typename std::enable_if<is_attribute<T>::value, void>::type
    serialize(Json::Value& node, const T &value, const std::string &key, const T &default_value)
    {
        add_attribute(node, key, value, default_value);
    }

    template<class T>
    typename std::enable_if<is_enum<T>::value, void>::type
    serialize(Json::Value& node, const T &value, const std::string &key)
    {
        add_attribute(node, key.empty() ? std::string("value") : key, value.str(), default_value::value<std::string>());
    }

    template<class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    serialize(Json::Value& node, const T *value, const std::string &key)
    {
        if (value)
        {
            add_attribute(node, key, value->name, default_value::value<std::string>());
        }
    }

    template<class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    serialize(Json::Value& node, const intrusive_ptr<T> &value, const std::string &key)
    {
        if (value)
        {
            Json::Value& child = key.empty() ? node : add_child(node, key);
            add_attribute(child, "type", value->get_type(), "");
            value->serialize_json(child);
        }
    }

    template<class T>
    typename std::enable_if<!is_attribute<T>::value && !is_enum<T>::value && !is_map<T>::value, void>::type
    serialize(Json::Value& node, const T &value, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : add_child(node, key);
        value.serialize_json(child);
    }

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        ((is_attribute<T>::value && !std::is_same<T, bool>::value) || is_enum<T>::value),
        void>::type
    serialize(Json::Value& node, const Set<T, Args...> &values, const std::string &key);

    template<template<class...> class Set, class... Args>
    typename std::enable_if<
        is_set_container<Set<bool, Args...>>::value,
        void>::type
    serialize(Json::Value& node, const Set<bool, Args...> &values, const std::string &key);

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_data<T>::value,
        void>::type
    serialize(Json::Value& node, const Set<T, Args...> &values, const std::string &key);

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_not_serialize_to_attribute<T>::value,
        void>::type
    serialize(Json::Value& node, const Set<T, Args...> &values, const std::string &key);
/* Vectors serialization start */
    template<class T>
    typename std::enable_if<(is_attribute<T>::value && !std::is_same<T, bool>::value) || is_enum<T>::value, void>::type
    serialize(Json::Value& node, const std::vector<T> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (const T& value : values)
        {
            add_array_item(child, value);
        }
    }
    template<class T>
    typename std::enable_if<is_attribute<T>::value && std::is_same<T, bool>::value>::type
    serialize(Json::Value& node, const std::vector<T> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (T value : values)
        {
            add_array_item(child, value);
        }
    }
    template<class T>
    typename std::enable_if<is_data<T>::value, void>::type
    serialize(Json::Value& node, const std::vector<T> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (const T &value : values)
        {
            add_array_item(child, value ? value->name : "");
        }
    }

    template<class T>
    typename std::enable_if<is_not_serialize_to_attribute<T>::value, void>::type
    serialize(Json::Value& node, const std::vector<T> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (const T &value : values)
        {
            Json::Value& item = add_array_item(child);
            serialize(item, value, "");
        }
    }
/* Vectors serialization finish */
/* Set serialization start */
    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        ((is_attribute<T>::value && !std::is_same<T, bool>::value) || is_enum<T>::value),
        void>::type
    serialize(Json::Value& node, const Set<T, Args...> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (const T& value : values)
        {
            add_array_item(child, value);
        }
    }
    template<template<class...> class Set, class... Args>
    typename std::enable_if<
        is_set_container<Set<bool, Args...>>::value,
        void>::type
    serialize(Json::Value& node, const Set<bool, Args...> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (bool value : values)
        {
            add_array_item(child, value);
        }
    }
    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_data<T>::value,
        void>::type
    serialize(Json::Value& node, const Set<T, Args...> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (const T &value : values)
        {
            add_array_item(child, value ? value->name : "");
        }
    }

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_not_serialize_to_attribute<T>::value,
        void>::type
    serialize(Json::Value& node, const Set<T, Args...> &values, const std::string &key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (const T &value : values)
        {
            Json::Value& item = add_array_item(child);
            serialize(item, value, "");
        }
    }
/* Set serialization finish */
/* Maps serialization start */
    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            serialize(item, pair.second, "value");
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            serialize(item, pair.second, "value");
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            serialize(item, pair.second, "value");
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            serialize(item, pair.first, "key");
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            serialize(item, pair.first, "key");
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            serialize(item, pair.first, "key");
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(Json::Value& node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        Json::Value& child = key.empty() ? node : add_array(node, key);
        for (auto& pair : values)
        {
            Json::Value& item = add_array_item(child);
            serialize(item, pair.first, "key");
            serialize(item, pair.second, "value");
        }
    }
/* Maps serialization finish */

    Json::Value& get_child(Json::Value& node, const std::string &name);

    int get_attribute(Json::Value& node, const std::string &key, int default_value = 0);
    int64_t get_attribute(Json::Value& node, const std::string &key, int64_t default_value = 0);
    unsigned int get_attribute(Json::Value& node, const std::string &key, unsigned int default_value = 0);
    uint64_t get_attribute(Json::Value& node, const std::string &key, uint64_t default_value = 0);
    bool get_attribute(Json::Value& node, const std::string &key, bool default_value = false);
    float get_attribute(Json::Value& node, const std::string &key, float default_value = 0.f);
    double get_attribute(Json::Value& node, const std::string &key, double default_value = 0.f);
    std::string get_attribute(Json::Value& node, const std::string &key, const std::string &default_value);

    void get_array_item(Json::Value& node, int &value);
    void get_array_item(Json::Value& node, int64_t &value);
    void get_array_item(Json::Value& node, unsigned int &value);
    void get_array_item(Json::Value& node, uint64_t &value);
    void get_array_item(Json::Value& node, bool &value);
    void get_array_item(Json::Value& node, float &value);
    void get_array_item(Json::Value& node, double &value);
    void get_array_item(Json::Value& node, std::string &value);

    template<class T>
    typename std::enable_if<is_attribute<T>::value, void>::type
    deserialize(Json::Value& node, T &value, const std::string &key, const T &default_value)
    {
        value = get_attribute(node, key, default_value);
    }

    template<class T>
    typename std::enable_if<is_enum<T>::value, void>::type
    deserialize(Json::Value& node, T &value, const std::string &key)
    {
        value = get_attribute(node, !key.empty() ? key : "value", default_value::value<std::string>());
    }

    template<class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    deserialize(Json::Value& node, const T *&value, const std::string &key)
    {
        value = DataStorage::shared().get<T>(get_attribute(node, key, default_value::value<std::string>()));
    }

    template<class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    deserialize(Json::Value& node, intrusive_ptr<T> &value, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        std::string type = get_attribute(child, std::string("type"), default_value::value<std::string>());
        value = Factory::shared().build<T>(get_attribute(child, "type", std::string()));
        if(value)
        {
            value->deserialize_json(child);
        }
    }

    template<class T>
    typename std::enable_if<!is_attribute<T>::value && !is_enum<T>::value && !is_map<T>::value, void>::type
    deserialize(Json::Value& node, T &value, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        value.deserialize_json(child);
    }

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_attribute<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key);

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_enum<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key);

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_data<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key);

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_not_serialize_to_attribute<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key);

/* Vectors deserialization start */
    template<class T>
    typename std::enable_if<is_attribute<T>::value, void>::type
    deserialize(Json::Value& node, std::vector<T> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            T value;
            get_array_item(item, value);
            values.push_back(value);
        }
    }

    template<class T>
    typename std::enable_if<is_enum<T>::value, void>::type
    deserialize(Json::Value& node, std::vector<T> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            std::string value_string;
            get_array_item(item, value_string);
            T value(value_string);
            values.push_back(value);
        }
    }

    template<class T>
    typename std::enable_if<is_data<T>::value, void>::type
    deserialize(Json::Value& node, std::vector<T> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            std::string value_string;
            get_array_item(item, value_string);
            T value = DataStorage::shared().get<typename data_type<T>::type>(value_string);
            values.push_back(value);
        }
    }

    template<class T>
    typename std::enable_if<is_not_serialize_to_attribute<T>::value, void>::type
    deserialize(Json::Value& node, std::vector<T> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            T value;
            deserialize(item, value, "");
            values.push_back(std::move(value));
        }
    }
/* Vectors deserialization finish */
/* Set deserialization start */
    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_attribute<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            T value;
            get_array_item(item, value);
            values.insert(value);
        }
    }

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_enum<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            std::string value_string;
            get_array_item(item, value_string);
            T value(value_string);
            values.insert(value);
        }
    }

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_data<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            std::string value_string;
            get_array_item(item, value_string);
            T value = DataStorage::shared().get<typename data_type<T>::type>(value_string);
            values.insert(value);
        }
    }

    template<template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_not_serialize_to_attribute<T>::value,
        void>::type
    deserialize(Json::Value& node, Set<T, Args...> &values, const std::string &key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            T value;
            deserialize(item, value, "");
            values.insert(value);
        }
    }
/* Set deserialization finish */
/* Maps deserialization start */
    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = value_;
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(Json::Value& node, Map& map, const std::string& key)
    {
        Json::Value& child = key.empty() ? node : get_child(node, key);
        for (Json::Value& item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = value_;
        }
    }
/* Maps deserialization finish */

}
}
#endif //__mg_serializer_json__

)__EXT__";

#pragma mark SERIALIZER_JSON_CPP
const std::string SERIALIZER_JSON_CPP = R"__EXT__(#include "SerializerJson.h"
#include "jsoncpp/json.h"


namespace mg
{
namespace serializer_json
{

Json::Value& add_child(Json::Value& node, const std::string &name)
{
    return node[name];
}

Json::Value& add_array(Json::Value& node, const std::string &name)
{
    return node[name];
}

Json::Value& add_array_item(Json::Value& node)
{
    return node.append(Json::Value());
}

Json::Value& get_child(Json::Value& node, const std::string &name)
{
    return node[name];
}

void add_attribute(Json::Value& node, const std::string &key, const int &value, int default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_attribute(Json::Value& node, const std::string &key, const int64_t &value, int64_t default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_attribute(Json::Value& node, const std::string &key, const unsigned int &value, unsigned int default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_attribute(Json::Value& node, const std::string &key, const uint64_t &value, uint64_t default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_attribute(Json::Value& node, const std::string &key, const bool &value, bool default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_attribute(Json::Value& node, const std::string &key, const float &value, float default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_attribute(Json::Value& node, const std::string &key, const double &value, double default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_attribute(Json::Value& node, const std::string &key, const std::string &value, const std::string &default_value)
{
    if (value != default_value)
    {
        node[key] = value;
    }
}

void add_array_item(Json::Value& node, const int &value)
{
    node.append(value);
}

void add_array_item(Json::Value& node, const int64_t &value)
{
    node.append(value);
}

void add_array_item(Json::Value& node, const unsigned int &value)
{
    node.append(value);
}

void add_array_item(Json::Value& node, const uint64_t &value)
{
    node.append(value);
}

void add_array_item(Json::Value& node, const bool &value)
{
    node.append(value);
}

void add_array_item(Json::Value& node, const float &value)
{
    node.append(value);
}

void add_array_item(Json::Value& node, const double &value)
{
    node.append(value);
}

void add_array_item(Json::Value& node, const std::string &value)
{
    node.append(value);
}


int get_attribute(Json::Value& node, const std::string &key, int default_value)
{
    return node.isMember(key) ? node[key].asInt() : default_value;
}

int64_t get_attribute(Json::Value& node, const std::string &key, int64_t default_value)
{
    return node.isMember(key) ? node[key].asInt64() : default_value;
}

unsigned int get_attribute(Json::Value& node, const std::string &key, unsigned int default_value)
{
    return node.isMember(key) ? node[key].asUInt() : default_value;
}

uint64_t get_attribute(Json::Value& node, const std::string &key, uint64_t default_value)
{
    return node.isMember(key) ? node[key].asUInt64() : default_value;
}

bool get_attribute(Json::Value& node, const std::string &key, bool default_value)
{
    return node.isMember(key) ? node[key].asBool() : default_value;
}

float get_attribute(Json::Value& node, const std::string &key, float default_value)
{
    return node.isMember(key) ? node[key].asFloat() : default_value;
}

double get_attribute(Json::Value& node, const std::string &key, double default_value)
{
    return node.isMember(key) ? node[key].asFloat() : default_value;
}

std::string get_attribute(Json::Value& node, const std::string &key, const std::string &default_value)
{
    return node.isMember(key) ? node[key].asString() : default_value;
}

void get_array_item(Json::Value& node, int &value)
{
    value = node.asInt();
}

void get_array_item(Json::Value& node, int64_t &value)
{
    value = node.asInt64();
}

void get_array_item(Json::Value& node, unsigned int &value)
{
    value = node.asUInt();
}

void get_array_item(Json::Value& node, uint64_t &value)
{
    value = node.asUInt64();
}

void get_array_item(Json::Value& node, bool &value)
{
    value = node.asBool();
}

void get_array_item(Json::Value& node, float &value)
{
    value = node.asFloat();
}

void get_array_item(Json::Value& node, double &value)
{
    value = node.asDouble();
}

void get_array_item(Json::Value& node, std::string &value)
{
    value = node.asString();
}

}

}
)__EXT__";
