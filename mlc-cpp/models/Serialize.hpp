//
//  Serialize.hpp
//  mlc-cpp
//
//  Common serialization format enum and helpers
//

#pragma once

#include <string>
#include <vector>
#include <array>

// Битовые флаги для форматов сериализации
enum class SerializeFormat {
    Xml = 1 << 0,
    Json = 1 << 1,
};

std::array<SerializeFormat, 2> SerializeFormat_getAll();
std::string SerializeFormat_to_str(SerializeFormat format);

inline int operator|(SerializeFormat a, SerializeFormat b) {
    return static_cast<int>(a) | static_cast<int>(b);
}

inline bool hasFlag(int flags, SerializeFormat f) {
    return (flags & static_cast<int>(f)) != 0;
}

// Возвращает все поддерживаемые форматы
inline std::vector<std::pair<SerializeFormat, std::string>> getAllSerializeFormats() {
    return {{SerializeFormat::Xml, "xml"}, {SerializeFormat::Json, "json"}};
}

