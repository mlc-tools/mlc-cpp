//
//  serializer_xml.h
//  mlc
//
//  Created by  vladimirtolmachev on 27.04.2026.
//
#include <string>

#pragma mark SERIALIZER_XML_HPP

const std::string SERIALIZER_XML_HPP = R"__EXT__(#ifndef __mg_serialize_xml_h__
#define __mg_serialize_xml_h__

#include <string>
#include <map>
#include <vector>
#include <set>
#include "intrusive_ptr.h"
#include "SerializerCommon.h"
#include "DataStorage.h"
#include "mg_Factory.h"
{include_custom_serializer}

namespace pugi
{
    class xml_node;
    class xml_document;
}

namespace mg
{
namespace serializer_xml
{
    pugi::xml_node add_child(pugi::xml_node node, const std::string& name);

    void add_attribute(pugi::xml_node node, const std::string& key, const int& value, int default_value=0);
    void add_attribute(pugi::xml_node node, const std::string& key, const int64_t& value, int64_t default_value=0);
    void add_attribute(pugi::xml_node node, const std::string& key, const unsigned int& value, unsigned int default_value=0);
    void add_attribute(pugi::xml_node node, const std::string& key, const uint64_t& value, uint64_t default_value=0);
    void add_attribute(pugi::xml_node node, const std::string& key, const bool& value, bool default_value=false);
    void add_attribute(pugi::xml_node node, const std::string& key, const float& value, float default_value=0.f);
    void add_attribute(pugi::xml_node node, const std::string& key, const double& value, double default_value=0.f);
    void add_attribute(pugi::xml_node node, const std::string& key, const std::string& value, const std::string& default_value);

    template <class T>
    typename std::enable_if<is_attribute<T>::value, void>::type
    serialize(pugi::xml_node node, const T& value, const std::string& key, const T& default_value)
    {
        add_attribute(node, key, value, default_value);
    }

    template <class T>
    typename std::enable_if<is_enum<T>::value, void>::type
    serialize(pugi::xml_node node, const T& value, const std::string& key)
    {
        add_attribute(node, key.empty() ? std::string("value") : key, value.str(), default_value::value<std::string>());
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    serialize(pugi::xml_node node, const T *value, const std::string& key)
    {
        if (value)
        {
            add_attribute(node, key, value->name, default_value::value<std::string>());
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value && !is_enum<T>::value && !is_map<T>::value, void>::type
    serialize(pugi::xml_node node, const intrusive_ptr<T>& value, const std::string& key)
    {
        if (value)
        {
            auto child = key.empty() ? node : add_child(node, key);
            add_attribute(child, "type", value->get_type(), "");
            value->serialize_xml(child);
        }
    }

    template <class T>
    typename std::enable_if<is_serializable<T>::value, void>::type 
    serialize(pugi::xml_node node, const T& value, const std::string& key)
    {
        auto child = key.empty() ? node : add_child(node, key);
        value.serialize_xml(child);
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_attribute<T>::value &&
        !std::is_same<T, bool>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<T, Args...>& values, const std::string& key);

    template <template<class...> class Set, class... Args>
    typename std::enable_if<
        is_set_container<Set<bool, Args...>>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<bool, Args...>& values, const std::string& key);

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<intrusive_ptr<T>, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<intrusive_ptr<T>, Args...>& values, const std::string& key);

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        !is_attribute<T>::value &&
        !is_intrusive<T>::value &&
        !std::is_pointer<T>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<T, Args...>& values, const std::string& key);

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<const T*, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<const T*, Args...>& values, const std::string& key);

/* Vectors serialization start */
    template <class T>
    typename std::enable_if<is_attribute<T>::value && !std::is_same<T, bool>::value, void>::type
    serialize(pugi::xml_node node, const std::vector<T>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const T& value : values)
        {
            auto item = add_child(child, "item");
            serialize(item, value, "value", default_value::value<T>());
        }
    }

    template <class T>
    typename std::enable_if<is_attribute<T>::value && std::is_same<T, bool>::value, void>::type
    serialize(pugi::xml_node node, const std::vector<T>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (T value : values)
        {
            auto item = add_child(child, "item");
            serialize(item, value, "value", default_value::value<T>());
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    serialize(pugi::xml_node node, const std::vector<intrusive_ptr<T>>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const intrusive_ptr<T>& value : values)
        {
            auto item = add_child(child, value ? value->get_type() : "");
            if(value)
            {
                value->serialize_xml(item);
            }
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    serialize(pugi::xml_node node, const std::vector<T>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const T& value : values)
        {
            auto item = add_child(child, "item");
            serializer_xml::serialize(item, value, "");
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    serialize(pugi::xml_node node, const std::vector<const T*>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const T* value : values)
        {
            auto item = add_child(child, "item");
            serialize(item, value, "value");
        }
    }
/* Vectors serialization finish */
/* Set serialization start */
    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_attribute<T>::value &&
        !std::is_same<T, bool>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<T, Args...>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const T& value : values)
        {
            auto item = add_child(child, "item");
            serialize(item, value, "value", default_value::value<T>());
        }
    }

    template <template<class...> class Set, class... Args>
    typename std::enable_if<
        is_set_container<Set<bool, Args...>>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<bool, Args...>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (bool value : values)
        {
            auto item = add_child(child, "item");
            serialize(item, value, "value", default_value::value<bool>());
        }
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<intrusive_ptr<T>, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<intrusive_ptr<T>, Args...>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const intrusive_ptr<T>& value : values)
        {
            auto item = add_child(child, value ? value->get_type() : "");
            if(value)
            {
                value->serialize_xml(item);
            }
        }
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        !is_attribute<T>::value &&
        !is_intrusive<T>::value &&
        !std::is_pointer<T>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<T, Args...>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const T& value : values)
        {
            auto item = add_child(child, "item");
            serializer_xml::serialize(item, value, "");
        }
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<const T*, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    serialize(pugi::xml_node node, const Set<const T*, Args...>& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (const T* value : values)
        {
            auto item = add_child(child, "item");
            serialize(item, value, "value");
        }
    }
/* Set serialization finish */
/* Maps serialization start */
    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first, default_value::value<typename Map::key_type>());
            serialize(item, pair.second, "value");
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first.str(), default_value::value<std::string>());
            serialize(node, pair.second, "value");
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            add_attribute(item, "key", pair.first ? pair.first->name : "", default_value::value<std::string>());
            serialize(node, pair.second, "value");
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            serialize(node, pair.first, "key");
            add_attribute(item, "value", pair.second, default_value::value<typename Map::mapped_type>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            serialize(node, pair.first, "key");
            add_attribute(item, "value", pair.second.str(), default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            serialize(node, pair.first, "key");
            add_attribute(item, "value", pair.second ? pair.second->name : "", default_value::value<std::string>());
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    serialize(pugi::xml_node node, const Map& values, const std::string& key)
    {
        if (values.empty())
            return;
        auto child = key.empty() ? node : add_child(node, key);
        for (auto& pair : values)
        {
            auto item = add_child(child, "pair");
            serialize(item, pair.first, "key");
            serialize(item, pair.second, "value");
        }
    }
/* Maps serialization finish */
    
    /* Deserialize */
    pugi::xml_node get_child(pugi::xml_node node, const std::string& name);

    int get_attribute(pugi::xml_node node, const std::string& key, int default_value=0);
    int64_t get_attribute(pugi::xml_node node, const std::string& key, int64_t default_value=0);
    unsigned int get_attribute(pugi::xml_node node, const std::string& key, unsigned int default_value=0);
    uint64_t get_attribute(pugi::xml_node node, const std::string& key, uint64_t default_value=0);
    bool get_attribute(pugi::xml_node node, const std::string& key, bool default_value=false);
    float get_attribute(pugi::xml_node node, const std::string& key, float default_value=0.f);
    double get_attribute(pugi::xml_node node, const std::string& key, double default_value=0.f);
    std::string get_attribute(pugi::xml_node node, const std::string& key, const std::string& default_value);

    template <class T>
    typename std::enable_if<is_attribute<T>::value, void>::type
    deserialize(pugi::xml_node node, T& value, const std::string& key, const T& default_value)
    {
        value = get_attribute(node, key, default_value);
    }

    template <class T>
    typename std::enable_if<is_enum<T>::value, void>::type
    deserialize(pugi::xml_node node, T& value, const std::string& key)
    {
        value = get_attribute(node, !key.empty() ? key : "value", default_value::value<std::string>());
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    deserialize(pugi::xml_node node, T const *&value, const std::string& key)
    {
        value = DataStorage::shared().get<T>(get_attribute(node, key, default_value::value<std::string>()));
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    deserialize(pugi::xml_node node, intrusive_ptr<T>& value, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        auto type = get_attribute(child, "type", std::string());
        if(!type.empty())
        {
            value = Factory::shared().build<T>(type);
            if(value)
            {
                value->deserialize_xml(child);
            }
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value && !is_enum<T>::value && !is_map<T>::value, void>::type
    deserialize(pugi::xml_node node, T& value, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        value.deserialize_xml(child);
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_attribute<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<T, Args...>& values, const std::string& key);

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<intrusive_ptr<T>, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<intrusive_ptr<T>, Args...>& values, const std::string& key);

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<const T*, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<const T*, Args...>& values, const std::string& key);

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        !is_attribute<T>::value &&
        !is_intrusive<T>::value &&
        !std::is_pointer<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<T, Args...>& values, const std::string& key);

/* Vectors deserialization start */
    template <class T>
    typename std::enable_if<is_attribute<T>::value, void>::type
    deserialize(pugi::xml_node node, std::vector<T>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            T value;
            deserialize(item, value, "value", default_value::value<T>());
            values.push_back(value);
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    deserialize(pugi::xml_node node, std::vector<intrusive_ptr<T>>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            std::string type = item.name();
            intrusive_ptr<T> object = Factory::shared().build<T>(type);
            if(object)
            {
                object->deserialize_xml(item);
            }
            values.push_back(object);
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    deserialize(pugi::xml_node node, std::vector<const T*>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            const T* value = DataStorage::shared().get<T>(get_attribute(item, "value", default_value::value<std::string>()));
            values.push_back(value);
        }
    }

    template <class T>
    typename std::enable_if<!is_attribute<T>::value, void>::type
    deserialize(pugi::xml_node node, std::vector<T>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            T value;
            deserialize(item, value, default_value::value<std::string>());
            values.push_back(std::move(value));
        }
    }
/* Vectors deserialization finish */
/* Set deserialization start */
    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        is_attribute<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<T, Args...>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            T value;
            deserialize(item, value, "value", default_value::value<T>());
            values.insert(value);
        }
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<intrusive_ptr<T>, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<intrusive_ptr<T>, Args...>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            std::string type = item.name();
            intrusive_ptr<T> object = Factory::shared().build<T>(type);
            if(object)
            {
                object->deserialize_xml(item);
            }
            values.insert(object);
        }
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<const T*, Args...>>::value &&
        !is_attribute<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<const T*, Args...>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            const T* value = DataStorage::shared().get<T>(get_attribute(item, "value", default_value::value<std::string>()));
            values.insert(value);
        }
    }

    template <template<class...> class Set, class T, class... Args>
    typename std::enable_if<
        is_set_container<Set<T, Args...>>::value &&
        !is_attribute<T>::value &&
        !is_intrusive<T>::value &&
        !std::is_pointer<T>::value,
        void>::type
    deserialize(pugi::xml_node node, Set<T, Args...>& values, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            T value;
            deserialize(item, value, default_value::value<std::string>());
            values.insert(value);
        }
    }
/* Set deserialization finish */
/* Maps deserialization start */
    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = get_attribute(item, "key", default_value::value<typename Map::key_type>());
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_enum<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_data<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_ = DataStorage::shared().get<typename data_type<typename Map::key_type>::type>(get_attribute(item, "key", default_value::value<std::string>()));
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = get_attribute(item, "value", default_value::value<typename Map::mapped_type>());
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_enum<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_data<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_ = DataStorage::shared().get<typename data_type<typename Map::mapped_type>::type>(get_attribute(item, "value", default_value::value<std::string>()));
            map[key_] = std::move(value_);
        }
    }

    template<class Map, class = std::void_t<typename Map::key_type, typename Map::mapped_type>>
    typename std::enable_if<is_not_serialize_to_attribute<typename Map::key_type>::value && is_not_serialize_to_attribute<typename Map::mapped_type>::value, void>::type
    deserialize(pugi::xml_node node, Map& map, const std::string& key)
    {
        auto child = key.empty() ? node : get_child(node, key);
        for (auto item : child)
        {
            typename Map::key_type key_; deserialize(item, key_, "key");
            typename Map::mapped_type value_; deserialize(item, value_, "value");
            map[key_] = std::move(value_);
        }
    }
/* Maps deserialization finish */

}

}
#endif

)__EXT__";

#pragma mark SERIALIZER_XML_CPP
const std::string SERIALIZER_XML_CPP = R"__EXT__(#include "SerializerXml.h"
#include "pugixml/pugixml.hpp"

namespace mg
{

namespace serializer_xml
{

pugi::xml_node add_child(pugi::xml_node node, const std::string &name)
{
    return node.append_child(name.c_str());
}

void add_attribute(pugi::xml_node node, const std::string &key, const int &value, int default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value);
    }
}

void add_attribute(pugi::xml_node node, const std::string &key, const int64_t &value, int64_t default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value);
    }
}

void add_attribute(pugi::xml_node node, const std::string &key, const unsigned int &value, unsigned int default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value);
    }
}

void add_attribute(pugi::xml_node node, const std::string &key, const uint64_t &value, uint64_t default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value);
    }
}

void add_attribute(pugi::xml_node node, const std::string &key, const bool &value, bool default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value);
    }
}

void add_attribute(pugi::xml_node node, const std::string &key, const float &value, float default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value);
    }
}

void add_attribute(pugi::xml_node node, const std::string &key, const double &value, double default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value);
    }
}

void add_attribute(pugi::xml_node node, const std::string &key, const std::string &value, const std::string &default_value)
{
    if (value != default_value)
    {
        node.append_attribute(key.c_str()).set_value(value.c_str());
    }
}



pugi::xml_node get_child(pugi::xml_node node, const std::string &name)
{
    return node.child(name.c_str());
}

int get_attribute(pugi::xml_node node, const std::string &key, int default_value)
{
    return node.attribute(key.c_str()).as_int(default_value);
}

int64_t get_attribute(pugi::xml_node node, const std::string &key, int64_t default_value)
{
    return node.attribute(key.c_str()).as_llong(default_value);
}

unsigned int get_attribute(pugi::xml_node node, const std::string &key, unsigned int default_value)
{
    return node.attribute(key.c_str()).as_uint(default_value);
}

uint64_t get_attribute(pugi::xml_node node, const std::string &key, uint64_t default_value)
{
    return node.attribute(key.c_str()).as_ullong(default_value);
}

bool get_attribute(pugi::xml_node node, const std::string &key, bool default_value)
{
    return node.attribute(key.c_str()).as_bool(default_value);
}

float get_attribute(pugi::xml_node node, const std::string &key, float default_value)
{
    return node.attribute(key.c_str()).as_float(default_value);
}

double get_attribute(pugi::xml_node node, const std::string &key, double default_value)
{
    return node.attribute(key.c_str()).as_double(default_value);
}

std::string get_attribute(pugi::xml_node node, const std::string &key, const std::string &default_value)
{
    return node.attribute(key.c_str()).as_string(default_value.c_str());
}

}

}

)__EXT__";
