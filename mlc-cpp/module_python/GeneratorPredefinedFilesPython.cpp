//
//  GeneratorPredefinedFilesPython.cpp
//  mlc-cpp
//

#include "GeneratorPredefinedFilesPython.hpp"
#include "../core/WriterBase.hpp"
#include "../models/Model.hpp"
#include "constants_serializers_py.hpp"

void GeneratorPredefinedFilesPython::generate(Model &model) {
    WriterBase writer;
    writer.set_model(model);

    // Provide import shims expected by WriterPython
    model.addFile(nullptr, "IntrusivePtr.py", py_runtime::INTRUSIVE);
    model.addFile(nullptr, "DataWrapper.py", py_runtime::DATA_WRAPPER);
    model.addFile(nullptr, "Meta.py", py_runtime::META);
    model.addFile(nullptr, "SerializerXml.py", py_runtime::SERIALIZER_XML);
    model.addFile(nullptr, "DeserializerXml.py", py_runtime::DESERIALIZER_XML);
    model.addFile(nullptr, "SerializerJson.py", py_runtime::SERIALIZER_JSON);
    model.addFile(nullptr, "DeserializerJson.py", py_runtime::DESERIALIZER_JSON);

    // Minimal common and mg_extensions (as before)
    static const std::string COMMON_XML = R"__(

def create_command_from_xml(string):
    from .DeserializerXml import DeserializerXml
    import xml.etree.ElementTree as ET
    from .Factory import Factory
    root = ET.fromstring(string)
    type = root.tag
    command = Factory.build(type)
    if command is not None:
        deserializer = DeserializerXml(root)
        command.deserialize_xml(deserializer)
    return command


def serialize_command_to_xml(command):
    from .SerializerXml import SerializerXml
    import xml.etree.ElementTree as ET
    root = ET.Element(command.get_type())
    serializer = SerializerXml(root)
    command.serialize_xml(serializer)
    return ET.tostring(root)


def clone_object(obj, _=None):
    payload = serialize_command_to_xml(obj)
    clone = create_command_from_xml(payload)
    return clone
)__";
    static const std::string COMMON_JSON = R"__(
def create_command_from_json(string):
    import json
    from .Factory import Factory
    from .DeserializerJson import DeserializerJson
    dictionary = json.loads(string)
    for key in dictionary:
        command = Factory.build(key)
        if command is not None:
            deserializer = DeserializerJson(dictionary[key])
            command.deserialize_json(deserializer)
        return command


def serialize_command_to_json(command):
    import json
    from .SerializerJson import SerializerJson
    js = dict()
    js[command.get_type()] = dict()
    serializer = SerializerJson(js[command.get_type()])
    command.serialize_json(serializer)
    return json.dumps(js)

def clone_object(obj, _=None):
    payload = serialize_command_to_json(obj)
    clone = create_command_from_json(payload)
    return clone
)__";
    std::string content;
    if (model.config.serializeFormats & static_cast<int>(SerializeFormat::Xml))
        content += COMMON_XML;
    if (model.config.serializeFormats & static_cast<int>(SerializeFormat::Json))
        content += COMMON_JSON;

    model.addFile(nullptr, "common.py", content);

    static const char *MG_EXT =
        "def strTo(value, class_):\n"
        "    if class_ == bool:\n"
        "        return value.lower() in ['yes', 'true', 'y']\n"
        "    return class_(value)\n\n"
        "def in_list(key, container):\n"
        "    return key in container\n\n"
        "def list_push(container, value):\n"
        "    container.append(value)\n\n"
        "def list_insert(container, value, index):\n"
        "    container.insert(value, index)\n\n"
        "def list_remove(container, value):\n"
        "    container.remove(value)\n\n"
        "def list_erase(container, index):\n"
        "    container.remove(container[index])\n\n"
        "def list_truncate(container, size):\n"
        "    while len(container) > size:\n"
        "        container.pop()\n\n"
        "def list_clear(container):\n"
        "    container.clear()\n\n"
        "def list_size(container):\n"
        "    return len(container)\n\n"
        "def list_index(container, item):\n"
        "    return container.index(item) if item in container else -1\n\n"
        "def list_resize(container, size):\n"
        "    list_truncate(container, size)\n"
        "    while len(container) < size:\n"
        "        container.append(None)\n\n"
        "def in_map(key, container):\n"
        "    return key in container\n\n"
        "def map_size(container):\n"
        "    return len(container)\n\n"
        "def map_remove(container, value):\n"
        "    container.pop(value, None)\n\n"
        "def map_clear(container):\n"
        "    container.clear()\n\n"
        "def string_empty(container):\n"
        "    return len(container) == 0\n\n"
        "def string_size(container):\n"
        "    return len(container)\n\n"
        "def split(string, delimiter):\n"
        "    return string.split(delimiter)\n\n"
        "def join(values, delimiter):\n"
        "    return delimiter.join(values)\n";
    model.addFile(nullptr, "mg_extensions.py", MG_EXT);
}
