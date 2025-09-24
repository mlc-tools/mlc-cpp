//  ArgsParser.cpp

#include "ArgsParser.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace Cli {

static bool starts_with(std::string_view s, std::string_view prefix) {
    return s.size() >= prefix.size() && s.substr(0, prefix.size()) == prefix;
}

ArgParser& ArgParser::addFlag(std::string long_name, char short_name, std::string help,
                              bool required) {
    OptionSpec spec{std::move(long_name), short_name, /*takes_value*/false, /*multi*/false,
                    std::move(help), /*default_value*/"", /*required*/required};
    OptionState st{spec};
    _short_to_long.erase(spec.short_name);
    _options[spec.long_name] = std::move(st);
    if (short_name) _short_to_long[short_name] = spec.long_name;
    return *this;
}

ArgParser& ArgParser::addOption(std::string long_name, char short_name, std::string help,
                                bool multi, std::string default_value,
                                bool required) {
    OptionSpec spec{std::move(long_name), short_name, /*takes_value*/true, multi,
                    std::move(help), std::move(default_value), /*required*/required};
    OptionState st{spec};
    if (!spec.default_value.empty()) st.values.push_back(spec.default_value);
    _short_to_long.erase(spec.short_name);
    _options[spec.long_name] = std::move(st);
    if (short_name) _short_to_long[short_name] = spec.long_name;
    return *this;
}

ArgParser& ArgParser::require(const std::string& long_name) {
    auto it = _options.find(long_name);
    if (it != _options.end()) it->second.spec.required = true;
    return *this;
}

ArgParser::OptionState* ArgParser::findByLong(const std::string& name) {
    auto it = _options.find(name);
    return it == _options.end() ? nullptr : &it->second;
}

ArgParser::OptionState* ArgParser::findByShort(char name) {
    auto it = _short_to_long.find(name);
    if (it == _short_to_long.end()) return nullptr;
    return findByLong(it->second);
}

const ArgParser::OptionState* ArgParser::findByLong(const std::string& name) const {
    auto it = _options.find(name);
    return it == _options.end() ? nullptr : &it->second;
}

const ArgParser::OptionState* ArgParser::findByShort(char name) const {
    auto it = _short_to_long.find(name);
    if (it == _short_to_long.end()) return nullptr;
    auto jt = _options.find(it->second);
    return jt == _options.end() ? nullptr : &jt->second;
}

bool ArgParser::parse(int argc, char** argv) {
    _error.clear();
    _positional.clear();

    bool end_of_opts = false; // после "--" — только позиционные

    for (int i = 1; i < argc; ++i) {
        std::string_view token = argv[i] ? std::string_view(argv[i]) : std::string_view();
        if (token.empty()) continue;

        if (end_of_opts || !starts_with(token, "-")) {
            _positional.emplace_back(token);
            continue;
        }

        if (token == "--") { end_of_opts = true; continue; }

        if (starts_with(token, "--")) {
            // Формат: --name или --name=value
            std::string name;
            std::string value;
            size_t eq = token.find('=');
            if (eq == std::string::npos) {
                name = std::string(token.substr(2));
            } else {
                name = std::string(token.substr(2, eq - 2));
                value = std::string(token.substr(eq + 1));
            }

            OptionState* st = findByLong(name);
            if (!st) { _error = "Неизвестная опция: --" + name; return false; }

            st->present = true;
            if (!st->spec.takes_value) {
                if (!value.empty()) { _error = "Опция --" + name + " не принимает значение"; return false; }
                continue;
            }

            if (value.empty()) {
                // Значение берём из следующего аргумента
                if (i + 1 >= argc) { _error = "Для --" + name + " требуется значение"; return false; }
                value = argv[++i];
            }
            if (!st->spec.multi) {
                st->values.clear();
            }
            st->values.push_back(value);
            continue;
        }

        // Короткие флаги: -w или -o value. Комбинации -abc трактуем как -a -b -c (только флаги).
        if (starts_with(token, "-")) {
            if (token.size() < 2) { _error = "Пустой короткий флаг '-'"; return false; }
            // Разбираем пачку: -abc
            for (size_t k = 1; k < token.size(); ++k) {
                char ch = token[k];
                OptionState* st = findByShort(ch);
                if (!st) { _error = std::string("Неизвестная опция: -") + ch; return false; }
                st->present = true;
                if (st->spec.takes_value) {
                    // Значение может идти слитно (-oValue) или отдельным аргументом (-o Value)
                    std::string val;
                    if (k + 1 < token.size()) {
                        // Остаток строки — это значение
                        val = std::string(token.substr(k + 1));
                        k = token.size(); // завершаем разбор этого токена
                    } else {
                        if (i + 1 >= argc) { _error = std::string("Для -") + ch + " требуется значение"; return false; }
                        val = argv[++i];
                    }
                    if (!st->spec.multi) st->values.clear();
                    st->values.push_back(std::move(val));
                    break; // короткая опция со значением — завершаем текущий токен
                }
            }
            continue;
        }
    }
    // Проверка обязательных опций: требуем, чтобы они были явно указаны в командной строке
    for (const auto& kv : _options) {
        const auto& st = kv.second;
        if (st.spec.required && !st.present) {
            _error += "\nОтсутствует обязательная опция: --" + st.spec.long_name;
        }
    }

    return _error.empty();
}

bool ArgParser::has(const std::string& long_name) const {
    const auto* st = findByLong(long_name);
    return st && st->present;
}

std::string ArgParser::get(const std::string& long_name, const std::string& fallback) const {
    const auto* st = findByLong(long_name);
    if (!st) return fallback;
    if (st->values.empty()) return fallback;
    return st->values.back();
}

std::vector<std::string> ArgParser::getAll(const std::string& long_name) const {
    const auto* st = findByLong(long_name);
    if (!st) return {};
    return st->values;
}

std::string ArgParser::help(const std::string& program_name) const {
    // Сформируем строки и разобьём на обязательные и опциональные
    size_t width = 0;
    struct Row { std::string opt; std::string help; };
    std::vector<Row> required_rows;
    std::vector<Row> optional_rows;

    // Стабильный порядок: по long_name
    std::vector<std::string> names; names.reserve(_options.size());
    for (const auto& kv : _options) names.push_back(kv.first);
    std::sort(names.begin(), names.end());

    for (const auto& lname : names) {
        const auto& st = _options.at(lname);
        std::string left;
        if (st.spec.short_name) {
            left += "-";
            left.push_back(st.spec.short_name);
            left += ", ";
        } else {
            left += "    "; // выравнивание, если нет короткого
        }
        left += "--" + st.spec.long_name;
        if (st.spec.takes_value) left += " <value>";
        width = std::max(width, left.size());
        std::string desc = st.spec.help;
        if (st.spec.required)
            desc = "(required!) " + desc;
        if (st.spec.takes_value && !st.spec.default_value.empty()) {
            desc += " [default: " + st.spec.default_value + "]";
        }
        Row row{std::move(left), std::move(desc)};
        if (st.spec.required) required_rows.push_back(std::move(row));
        else optional_rows.push_back(std::move(row));
    }

    std::string out;
    out += "Usage: " + program_name + " [options] [--] [args...]\n\n";
    out += "Options:\n";
    // Сначала обязательные, потом опциональные
    for (const auto& r : required_rows) {
        out += "  " + r.opt;
        if (r.opt.size() < width) out.append(width - r.opt.size(), ' ');
        out += "  " + r.help + "\n";
    }
    for (const auto& r : optional_rows) {
        out += "  " + r.opt;
        if (r.opt.size() < width) out.append(width - r.opt.size(), ' ');
        out += "  " + r.help + "\n";
    }
    return out;
}

} // namespace Cli
