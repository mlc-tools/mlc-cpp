
#ifndef __mg_functions_h__
#define __mg_functions_h__

#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "config.h"
#include <assert.h>
#include <cstdarg>

#include "pugixml/pugixml.hpp"
#include "jsoncpp/json.h"
#include "mg_Factory.h"
#include "SerializerXml.h"
#include "SerializerJson.h"

namespace mg
{

    template <class K, class T, class P>
    bool in_map(const K& element, const std::map<T, P>& map)
    {
        return map.count(element) > 0;
    }

    template <class I, class T>
    bool in_list(I item, const std::vector<T>& list)
    {
        return std::find(list.begin(), list.end(), item) != list.end();
    }

    template <class T, class I>
    void list_push(std::vector<T>& list, const I& t)
    {
        list.push_back(t);
    }

    template <class T, class I>
    void list_insert(std::vector<T>& list, size_t index, const I& t)
    {
        assert(index <= list.size());
        list.insert(list.begin() + index, t);
    }

    template <class T, class I>
    void list_remove(std::vector<T>& list, const I& t)
    {
        auto iter = std::find(list.begin(), list.end(), t);
        if(iter != list.end())
            list.erase(iter);
    }

    template <class T>
    void list_erase(std::vector<T>& list, size_t index)
    {
        assert(index < list.size());
        list.erase(list.begin() + index);
    }

    template <class T>
    void list_truncate(std::vector<T>& list, size_t length)
    {
        assert(length < list.size());
        list.erase(list.begin() + length, list.end());
    }

    template <class T>
    int list_size(const std::vector<T>& vector)
    {
        return static_cast<int>(vector.size());
    }
    
    template <class T>
    int list_index(const std::vector<T>& list, const T& t)
    {
        auto iter = std::find(list.begin(), list.end(), t);
        if(iter != list.end())
            return iter - list.begin();
        return -1;
    }

    template <class T>
    void list_clear(std::vector<T>& vector)
    {
        vector.clear();
    }

    template <class T>
    void list_resize(std::vector<T>& vector, int size)
    {
        vector.resize(size);
    }

    template <class T, class P>
    int map_size(const std::map<T, P>& map)
    {
        return static_cast<int>(map.size());
    }
    template <class T, class P>
    void map_clear(std::map<T, P>& map)
    {
        map.clear();
    }
    template <class T, class P>
    void map_remove(std::map<T, P>& map, const T& key)
    {
        auto iter = map.find(key);
        if(iter != map.end())
        {
            map.erase(iter);
        }
    }

    bool string_empty(const std::string& string);
    int string_size(const std::string& string);

    float random_float();
    int random_int(int min, int max);

    template <class T>
    void mg_swap(T& a, T& b)
    {
        std::swap(a, b);
    }
    
    std::vector<std::string> split(const std::string& string, const char delimiter);
    std::string join(const std::vector<std::string>& values, const char delimiter);

    // Converters
    template <typename T> T strTo(const std::string &value);
    template <typename T> std::string toStr(T value);

    //XML
    template <class T> void set(pugi::xml_attribute& xml, T value);
    template <class T> T get(const pugi::xml_attribute& xml);

    template <class T> void set(pugi::xml_node& xml, const std::string& key, T value)
    {
        auto attribute = xml.append_attribute(key.c_str());
        set<T>(attribute, value);
    }
    template <class T> T get(const pugi::xml_node& xml, const std::string& key)
    {
        auto attribute = xml.attribute(key.c_str());
        if(attribute)
            return get<T>(attribute);
        return 0;
    }

    //JSON
    template <class T> void set(Json::Value& json, T value);
    template <class T> T get(const Json::Value& json);

    template <class T> void set(Json::Value& json, const std::string& key, T value)
    {
        set<T>(json[key], value);
    }
    template <class T> T get(const Json::Value& json, const std::string& key)
    {
        get<T>(json[key]);
    }

    std::string format(const char *fmt, ...);

    

    

    
    template <class TType>
    static std::string serialize_command_to_xml(const TType* command)
    {
        pugi::xml_document doc;
        auto root = doc.append_child(command->get_type().c_str());
        SerializerXml serializer(root);
        command->serialize_xml(serializer);

        std::stringstream stream;
        pugi::xml_writer_stream writer(stream);
#ifdef NDEBUG
        doc.save(writer,
                 "",
                 pugi::format_no_declaration | pugi::format_raw,
                 pugi::xml_encoding::encoding_utf8);
#else
        doc.save(writer,
                 PUGIXML_TEXT(" "),
                 pugi::format_no_declaration | pugi::format_indent,
                 pugi::xml_encoding::encoding_utf8);
#endif
        return stream.str();
    }

    template <class TType>
    static intrusive_ptr<TType> create_command_from_xml(const std::string& payload)
    {
        pugi::xml_document doc;
        doc.load_string(payload.c_str());
        auto root = doc.root().first_child();
        auto command = Factory::shared().build<TType>(root.name());
        DeserializerXml deserializer(root);
        if(command)
            command->deserialize_xml(deserializer);
        return command;
    }

    template <class TType>
    static std::string serialize_command_to_json(const TType* command)
    {
        Json::Value json;
        SerializerJson serializer(json[command->get_type()]);
        command->serialize_json(serializer);

        Json::StreamWriterBuilder wbuilder;
        wbuilder["indentation"] = "";
        return Json::writeString(wbuilder, json);
    }

    template <class TType>
    static intrusive_ptr<TType> create_command_from_json(const std::string& payload)
    {
        Json::Value json;
        Json::Reader reader;
        if(!reader.parse(payload, json))
        {
            return nullptr;
        }
        
        if(json.getMemberNames().size() == 0)
        {
            return nullptr;
        }

        auto type = json.getMemberNames()[0];
        DeserializerJson deserializer(json[type]);
        auto command = Factory::shared().build<TType>(type);
        if(command)
        {
            command->deserialize_json(deserializer);
        }
        return command;
    }

    template <class TType>
    static intrusive_ptr<TType> clone_object(const TType* object)
    {
        auto payload = serialize_command_to_json<TType>(object);
        auto clone = create_command_from_json<TType>(payload);
        return clone;
    }
    
    
    std::string fs_get_string(const std::string& path);
    
    template<typename T> struct Default { static constexpr T value = 0; };
    template<> struct Default<std::string> {static const std::string value;};
}

#endif
