//
//  GeneratorDataStoragePython.cpp
//

#include "GeneratorDataStoragePython.hpp"
#include "../models/Model.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Object.hpp"
#include "../models/Serialize.hpp"

GeneratorDataStoragePython::GeneratorDataStoragePython()
  : GeneratorDataStorageBase()
{}

void GeneratorDataStoragePython::generate(Model &model)
{
    GeneratorDataStorageBase::generate(model);
    if (model.config.serializeFormats & static_cast<int>(SerializeFormat::Xml)){
        addSerializeXml(model);
        addDeserializeXml(model);
    }
    if (model.config.serializeFormats & static_cast<int>(SerializeFormat::Json)){
        addSerializeJson(model);
        addDeserializeJson(model);
    }
}

std::string GeneratorDataStoragePython::getSharedMethodBody()
{
    return R"(
        if not DataStorage.__instance:
            DataStorage.__instance = DataStorage()
        return DataStorage.__instance)";
}

std::string GeneratorDataStoragePython::getPatternGetter()
{
    return R"__(
        if name == '':
            return None
        if not self._loaded and name not in self.{map}:
            from .{type} import {type}
            self.{map}[name] = {type}()
            self.{map}[name].name = name
        from .DataWrapper import DataWrapper
        return DataWrapper(self.{map}[name] if name in self.{map} else None))__";
}

std::string GeneratorDataStoragePython::getInitializeFunctionJsonBody()
{
    return R"__(
        js = json.loads(content)
        deserializer = DeserializerJson(js)
        self.deserialize_json(deserializer)
        self._loaded = True)__";
}

std::string GeneratorDataStoragePython::getInitializeFunctionXmlBody()
{
    return R"__(
        root = ET.fromstring(content)
        deserializer = DeserializerXml(root)
        self.deserialize_xml(deserializer)
        self._loaded = True)__";
}

void GeneratorDataStoragePython::addSerializeJson(Model &model){
    addSerializeMethod("serialize_json");
    auto *method = _class->get_method("serialize_json");
    if (!method)
        return;
    for (auto &kv : dataMembers_)
    {
        const auto &obj = kv.second;
        method->body += "\n        serializer.serialize(self." + obj.name + ", \"" + obj.name + "\")";
    }
    if(method->body.empty())
        method->body = "        pass";
}
void GeneratorDataStoragePython::addSerializeXml(Model &model){
    addSerializeMethod("serialize_xml");
    auto *method = _class->get_method("serialize_xml");
    if (!method)
        return;
    for (auto &kv : dataMembers_)
    {
        const auto &obj = kv.second;
        method->body += "\n        serializer.serialize(self." + obj.name + ", \"" + obj.name + "\")";
    }
    if(method->body.empty())
        method->body = "        pass";
}

void GeneratorDataStoragePython::addSerializeMethod(const std::string &name){
    
    Function method;
    method.name = name;
    method.translated = true;
    method.return_type = Objects::VOID;
    method.is_virtual = false;
    method.is_const = false;
    method.callable_args.emplace_back(Object("Serializer", "serializer"));
    _class->functions.push_back(std::move(method));
}

void GeneratorDataStoragePython::addDeserializeMethod(const std::string &name)
{
    Function method;
    method.name = name;
    method.translated = true;
    method.return_type = Objects::VOID;
    method.is_virtual = false;
    method.is_const = false;
    method.callable_args.emplace_back(Object("Deserializer", "deserializer"));
    _class->functions.push_back(std::move(method));
}

void GeneratorDataStoragePython::addDeserializeJson(Model & /*model*/)
{
    addDeserializeMethod("deserialize_json");
    auto *method = _class->get_method("deserialize_json");
    if (!method)
        return;
    for (auto &kv : dataMembers_)
    {
        const auto &obj = kv.second;
        const std::string getter = "get" + kv.first;
        const std::string mapName = obj.name;
        method->body += "\n        " + mapName + " = deserializer.json.get('" + mapName + "', None)";
        method->body += "\n        if " + mapName + ":";
        method->body += "\n            for o in " + mapName + ":";
        method->body += "\n                self." + getter + "(o['key'])";
        method->body += "\n            for o in " + mapName + ":";
        method->body += "\n                data = self." + getter + "(o['key'])";
        method->body += "\n                data.deserialize_json(DeserializerJson(o['value']))";
    }
    if(method->body.empty())
        method->body = "        pass";
}

void GeneratorDataStoragePython::addDeserializeXml(Model & /*model*/)
{
    addDeserializeMethod("deserialize_xml");
    auto *method = _class->get_method("deserialize_xml");
    if (!method)
        return;
    for (auto &kv : dataMembers_)
    {
        const auto &obj = kv.second;
        const std::string getter = "get" + kv.first;
        const std::string mapName = obj.name;
        method->body += "\n        " + mapName + " = deserializer.node.find('" + mapName + "')";
        method->body += "\n        if " + mapName + ":";
        method->body += "\n            for o in " + mapName + ":";
        method->body += "\n                self." + getter + "(o.attrib['key'])";
        method->body += "\n            for o in " + mapName + ":";
        method->body += "\n                data = self." + getter + "(o.attrib['key'])";
        method->body += "\n                if data:";
        method->body += "\n                    deserializer_data = DeserializerXml(o.find('value'))";
        method->body += "\n                    data.deserialize_xml(deserializer_data)";
    }
    if(method->body.empty())
        method->body = "        pass";
}
