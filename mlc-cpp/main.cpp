#include <iostream>
#include <fstream>
#include "models/Model.hpp"
#include "core/Parser.hpp"
#include "core/Linker.hpp"
#include "utils/FileUtils.hpp"
#include "utils/ArgsParser.hpp"
#include "tests/tests.hpp"
#include "Mlc.hpp"
#include <re2/re2.h>

std::string getFileContent(const std::string& path)
{
    std::fstream stream(path, std::ios::in);
    if (stream.is_open() == false)
        return "";
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
}

void log(const std::shared_ptr<Class>& cls){
    std::cout << "\nClass: " << cls->group << "/" << cls->name << "\n";
    std::cout << "Includes:\n";
    for (auto &i : cls->includes) std::cout << "  " << i << "\n";
    std::cout << "Members:\n";
    for (auto &m : cls->members) std::cout << "  " << m.type << " " << m.name << "\n";
    std::cout << "Methods:\n";
    for (auto &m : cls->functions) {
        std::cout << "  " << m.return_type.type << " " << m.name << "(";
        for (auto &a : m.callable_args) std::cout << a.type << " " << a.name << " ";
        std::cout << ")\n";
    }
    std::cout << "Constructors:\n";
    for (auto &m : cls->constructors) {
        std::cout << "  " << m.name << "(";
        for (auto &a : m.callable_args) std::cout << a.type << " " << a.name << " ";
        std::cout << ")\n";
    }
}

int main(int argc, char** argv) {
    tests::run();
    
    auto start = std::chrono::steady_clock::now();
    
    Mlc app;
//    app.setConfigsDirectory("/Users/vladimirtolmachev/Documents/xcode/mlc-cpp/test/configs");
//    app.setConfigsDirectory("/Users/vladimirtolmachev/work/archero/configs");
//    app.setConfigsDirectory("/Users/vladimirtolmachev/work/dungeon2/configs");

    {
        // Значения по умолчанию для локального прогона (могут быть переопределены флагами)
        app.setSide(Side::client);
        app.setgenerate_tests(false);
        app.setgenerate_intrusive(false);
//        app.setfilter_code([](const std::string& path){ return path.find("/unit_tests/") == std::string::npos; });

        Cli::ArgParser args;
        args.addOption("configs_directories", 'c', "Каталог(и) с .mlc, можно повторять", true);
        args.addOption("data_directories", 'd', "Каталог(и) с данными, можно повторять", true);
        args.addOption("out_directory", 0, "Каталог вывода кода");
        args.addOption("out_data_directory", 0, "Каталог вывода данных");
        args.addOption("language", 'l', "Целевой язык (например, cpp)");
        args.addOption("only_data", 0, "Только данные (true/false)");
        args.addOption("namespace_name", 'n', "Пространство имён для генерации");
        args.addOption("side", 's', "Сторона: client|server|both");
        args.addOption("php_validate", 0, "Проверка PHP (true/false)");
        args.addOption("allow_different_virtual", 0, "Разрешить разные virtual (true/false)");
        args.addOption("test_script", 0, "Путь к тест-скрипту");
        args.addOption("test_script_args", 0, "Аргументы для тест-скрипта");
        args.addOption("generate_tests", 0, "Генерировать тестовые классы (true/false)");
        args.addOption("generate_intrusive", 0, "Генерировать intrusive (true/false)");
        args.addOption("generate_factory", 0, "Генерировать фабрику (true/false)");
        args.addOption("serialize_protocol", 0, "Сериализация: список через запятую xml,json (можно повторять)", true);
        args.addOption("join_to_one_file", 0, "Объединять вывод в один файл (true/false)");
        args.addOption("auto_registration", 0, "Авто-регистрация (true/false)");
        args.addOption("generate_ref_counter", 0, "Генерировать рефкаунтер (true/false)");
        args.addOption("user_includes", 0, "Разрешить пользовательские include (true/false)");
        args.addOption("empty_methods", 0, "Генерировать пустые методы (true/false)");
        args.addOption("filter_code", 0, "RE2-паттерн(ы) включения; поддерживается префикс '!' для исключения (--filter_code='!unit_tests/')", true);
        args.addOption("filter_data", 0, "RE2-паттерн(ы) включения; поддерживается префикс '!' для исключения", true);
        args.addOption("custom_generator", 0, "Кастомный генератор (например, none)");
        args.addFlag("help", 'h', "Вывод справки");
        args.addFlag("watch", 'w', "Включить режим наблюдения за изменениями");
        
        args.require("configs_directories");
        args.require("data_directories");
        args.require("out_directory");
        args.require("out_data_directory");
        args.require("language");
        
        if (!args.parse(argc, argv)) {
            if(args.has("help")){
                std::cout << args.help("mlc") << std::endl;
                return 0;
            }
            std::cerr << args.error() << "\n\n" << args.help("mlc") << std::endl;
            return 1;
        }

        auto to_bool = [](const std::string& v, bool* ok = nullptr){
            std::string s; s.reserve(v.size());
            for(char ch : v) s.push_back(static_cast<char>(::tolower(static_cast<unsigned char>(ch))));
            bool val = (s == "1" || s == "true" || s == "yes" || s == "on");
            if (ok) *ok = (s=="1"||s=="0"||s=="true"||s=="false"||s=="yes"||s=="no"||s=="on"||s=="off");
            if (s == "0" || s == "false" || s == "no" || s == "off") val = false;
            return val;
        };

        // Применяем пути
        if (!args.getAll("configs_directories").empty()) {
            app.get_model().configs_directories.clear();
            for (auto &dir : args.getAll("configs_directories")) app.addConfigsDirectory(dir);
        }
        if (!args.getAll("data_directories").empty()) {
            app.get_model().data_directories.clear();
            for (auto &dir : args.getAll("data_directories")) app.addDataDirectory(dir);
        }
        if (args.has("out_directory")) app.setOutDirectory(args.get("out_directory"));
        if (args.has("out_data_directory")) app.setOutDataDirectory(args.get("out_data_directory"));

        // Общая конфигурация
        if (args.has("language")) app.setLanguage(args.get("language"));
        if (args.has("only_data")) app.setonly_data(to_bool(args.get("only_data")));
        if (args.has("namespace_name")) app.setNamespace(args.get("namespace_name"));
        if (args.has("side")) {
            std::string s = args.get("side");
            for(char& ch : s)
                ch = static_cast<char>(::tolower(static_cast<unsigned char>(ch)));
            if (s == "client")
                app.setSide(Side::client);
            else if (s == "server")
                app.setSide(Side::server);
            else
                app.setSide(Side::both);
        }
        if (args.has("php_validate")) app.setphp_validate(to_bool(args.get("php_validate")));
        if (args.has("allow_different_virtual")) app.get_model().allow_different_virtual = to_bool(args.get("allow_different_virtual"));
        if (args.has("test_script")) app.get_model().test_script = args.get("test_script");
        if (args.has("test_script_args")) app.get_model().test_script_args = args.get("test_script_args");

        // Генерация: булевы
        if (args.has("generate_tests")) app.setgenerate_tests(to_bool(args.get("generate_tests")));
        if (args.has("generate_intrusive")) app.setgenerate_intrusive(to_bool(args.get("generate_intrusive")));
        if (args.has("generate_factory")) app.setgenerate_factory(to_bool(args.get("generate_factory")));
        if (args.has("join_to_one_file")) app.setjoin_to_one_file(to_bool(args.get("join_to_one_file")));
        if (args.has("auto_registration")) app.setauto_registration(to_bool(args.get("auto_registration")));
        if (args.has("generate_ref_counter")) app.setgenerate_ref_counter(to_bool(args.get("generate_ref_counter")));
        if (args.has("user_includes")) app.setuser_includes(to_bool(args.get("user_includes")));
        if (args.has("empty_methods")) app.setempty_methods(to_bool(args.get("empty_methods")));

        // serialize_protocol: собираем маску и вектор форматов
        {
            auto raw = args.getAll("serialize_protocol");
            std::vector<std::string> tokens;
            for (auto &v : raw) {
                size_t start = 0, pos = 0;
                while ((pos = v.find(',', start)) != std::string::npos) {
                    tokens.push_back(v.substr(start, pos - start));
                    start = pos + 1;
                }
                if (start < v.size()) tokens.push_back(v.substr(start));
            }
            if (!tokens.empty()) {
                int mask = 0;
                std::vector<SerializeFormat> protos;
                for (auto &t : tokens) {
                    std::string s = t; for(char& ch : s) ch = static_cast<char>(::tolower(static_cast<unsigned char>(ch)));
                    if (s == "xml") { mask |= static_cast<int>(SerializeFormat::Xml); protos.push_back(SerializeFormat::Xml); }
                    else if (s == "json") { mask |= static_cast<int>(SerializeFormat::Json); protos.push_back(SerializeFormat::Json); }
                }
                if (mask != 0) {
                    app.setSerializeFormats(mask);
                    app.get_model().serialize_protocol = std::move(protos);
                }
            }
        }

        // Фильтры: RE2, допускается несколько и списки через запятую.
        // Логика: если совпал любой исключающий ('!') — отклонить;
        //         иначе если нет включающих — принять всё; иначе принять, если совпал любой включающий.
        {
            auto gather_patterns = [](const std::vector<std::string>& vals){
                std::vector<std::string> out; out.reserve(vals.size());
                for (auto &v : vals) {
                    size_t start = 0, pos = 0;
                    while ((pos = v.find(',', start)) != std::string::npos) {
                        auto tok = v.substr(start, pos - start);
                        if (!tok.empty())
                            out.push_back(tok);
                        start = pos + 1;
                    }
                    auto last = v.substr(start);
                    if (!last.empty())
                        out.push_back(last);
                }
                return out;
            };

            auto make_filter = [](const std::vector<std::string>& raws) -> std::function<bool(const std::string&)> {
                if (raws.empty()) return {};
                auto inc = std::make_shared<std::vector<std::unique_ptr<RE2>>>();
                auto exc = std::make_shared<std::vector<std::unique_ptr<RE2>>>();
                for (const auto &p : raws) {
                    if (!p.empty() && p[0] == '!')
                        exc->emplace_back(std::make_unique<RE2>(p.substr(1)));
                    else
                        inc->emplace_back(std::make_unique<RE2>(p));
                }
                return [inc, exc](const std::string& path){
                    for (const auto &re : *exc)
                        if (RE2::PartialMatch(path, *re))
                            return false;
                    if (inc->empty())
                        return true;
                    for (const auto &re : *inc)
                        if (RE2::PartialMatch(path, *re))
                            return true;
                    return false;
                };
            };

            if (auto f = make_filter(gather_patterns(args.getAll("filter_code"))); f) {
                app.setfilter_code(std::move(f));
            }
            if (auto f = make_filter(gather_patterns(args.getAll("filter_data"))); f) {
                app.setfilter_data(std::move(f));
            }
        }

        // custom_generator: пока поддерживаем только special 'none' для отключения
        if (args.has("custom_generator")) {
            std::string v = args.get("custom_generator");
            for(char& ch : v) ch = static_cast<char>(::tolower(static_cast<unsigned char>(ch)));
            if (v == "none") app.setUserGenerator(nullptr);
        }

        // Выполнение
        bool watch = args.has("watch");
        if (watch) {
            app.watchAndServe(/*poll_ms*/300, /*debounce_ms*/200);
        } else {
            if (app.get_model().only_data) {
                app.generateData();
            } else {
                app.generate();
                app.generateData();
            }
        }
    }
//    {
//        app.setSide(Side::client);
//        app.setgenerate_tests(false);
//        app.setgenerate_intrusive(false);
//        app.addConfigsDirectory("/Users/vladimirtolmachev/work/archero/configs/");
//        app.addDataDirectory("/Users/vladimirtolmachev/work/archero/configs/data/");
//        app.setfilter_code([](const std::string& path){
//            return path.find("/unit_tests/") == std::string::npos;
//        });
//        app.setOutDirectory("/Users/vladimirtolmachev/work/archero/client/generated/mg/");
//        app.setOutDataDirectory("/Users/vladimirtolmachev/work/archero/assets/data/");
//        app.generate();
//        app.generateData();
//    }
    
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    std::cout << "parsed on " << elapsed.count() << "ms" << std::endl;

    return 0;
}
