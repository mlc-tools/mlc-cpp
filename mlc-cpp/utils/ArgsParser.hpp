//  ArgsParser.hpp
//  Простой класс для парсинга аргументов командной строки

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Cli {

struct OptionSpec {
    std::string long_name;     // без ведущих «--», например: "watch"
    char short_name = 0;       // однобуквенный алиас, например: 'w' для -w
    bool takes_value = false;  // ожидает ли опция значение
    bool multi = false;        // допускается несколько значений
    std::string help;          // описание для помощи
    std::string default_value; // значение по умолчанию (для options)
    bool required = false;     // обязательно ли явно указывать опцию
};

class ArgParser {
public:
    // Регистрация флагов и опций
    ArgParser &addFlag(std::string long_name, char short_name, std::string help,
                       bool required = false);
    ArgParser &addOption(std::string long_name, char short_name,
                         std::string help, bool multi = false,
                         std::string default_value = "", bool required = false);

    // Отметить ранее добавленную опцию как обязательную
    ArgParser &require(const std::string &long_name);

    // Парсинг argv
    bool parse(int argc, char **argv);

    // Доступ к значениям
    bool has(const std::string &long_name) const; // наличие флага/опции
    std::string get(const std::string &long_name, // одно значение
                    const std::string &fallback = "") const;
    std::vector<std::string>
    getAll(const std::string &long_name) const; // все значения
    const std::vector<std::string> &positional() const { return _positional; }

    // Сообщения
    const std::string &error() const { return _error; }
    std::string help(const std::string &program_name) const;

private:
    struct OptionState {
        OptionSpec spec;
        bool present = false;            // был ли указан
        std::vector<std::string> values; // все значения опции
    };

    // Поиск по длинному и короткому имени
    OptionState *findByLong(const std::string &name);
    OptionState *findByShort(char name);
    const OptionState *findByLong(const std::string &name) const;
    const OptionState *findByShort(char name) const;

private:
    std::unordered_map<std::string, OptionState> _options; // ключ — long_name
    std::unordered_map<char, std::string>
        _short_to_long;                   // отображение short -> long
    std::vector<std::string> _positional; // позиционные аргументы
    std::string _error;
};

} // namespace Cli
