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
#include "utils/Config.hpp"
#include "utils/Common.hpp"
#include "version.hpp"

std::string getFileContent(const std::string& path)
{
    std::fstream stream(path, std::ios::in);
    if (stream.is_open() == false)
        return "";
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
}

void confugure_args(Cli::ArgParser& args, int argc, char** argv)
{
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
    args.addOption("config", 'j', "Путь к JSON-конфигу (например, mlc.json)");
    args.addFlag("watch", 'w', "Включить режим наблюдения за изменениями");
    args.addFlag("help", 'h', "Вывод справки");
    args.addFlag("version", 'v', "Version");

    if (!args.parse(argc, argv)) {
        std::cerr << args.error() << "\n\n" << args.help("mlc") << std::endl;
        exit(1);
    }
    
    if(args.has("help")){
        std::cout << args.help("mlc") << std::endl;
        exit(0);
    }
    if(args.has("version")){
        std::cout << APP_VERSION << std::endl;
        exit(0);
    }
}

bool try_load_config(Mlc& app, Cli::ArgParser& args){
    std::string cfg_path;
    if (args.has("config"))
        cfg_path = args.get("config");
    else if (FileUtils::exists("mlc.json"))
        cfg_path = "mlc.json";
    if (!cfg_path.empty()) {
        std::vector<Config> jobs; std::string err;
        auto config = Config::loadFile(cfg_path, err);
        if (!config) {
            std::cerr << "Failed to parse config '" << cfg_path << "': " << err << std::endl;
            return 1;
        }
        app.get_model().configuration = config;
        if(!config.jobs.empty())
            app.get_model().config = config.jobs.at(0);
        else
            return false;
        return true;
    }
    return false;
}

bool try_load_argc(Mlc& app, Cli::ArgParser& args){
    // Без конфига проверим обязательные ключи
    const char* required_opts[] = {"configs_directories","data_directories","out_directory","out_data_directory","language"};
    for (auto key : required_opts) {
        if (args.getAll(key).empty() && !args.has(key)) {
            std::cerr << "Missing required option --" << key << "\n\n" << args.help("mlc") << std::endl;
            return 1;
        }
    }

    Config configuration;
    Job job;

    // Источники
    if (!args.getAll("configs_directories").empty()) {
        configuration.configs_directories.clear();
        for (auto &dir : args.getAll("configs_directories"))
            configuration.configs_directories.push_back(FileUtils::normalizePath(dir));
    }
    if (!args.getAll("data_directories").empty()) {
        configuration.data_directories.clear();
        for (auto &dir : args.getAll("data_directories"))
            configuration.data_directories.push_back(dir);
    }

    // Параметры задачи
    if (args.has("out_directory"))
        job.out_directory = args.get("out_directory");
    if (args.has("out_data_directory"))
        job.out_data_directory = args.get("out_data_directory");
    if (args.has("language"))
        job.language = args.get("language");
    if (args.has("only_data"))
        job.only_data = to_bool(args.get("only_data"));
    if (args.has("namespace_name"))
        job.namespace_name = args.get("namespace_name");
    if (args.has("side")) {
        std::string s = args.get("side");
        for(char& ch : s)
            ch = static_cast<char>(::tolower(static_cast<unsigned char>(ch)));
        if (s == "client")
            job.side = Side::client;
        else if (s == "server")
            job.side = Side::server;
        else
            job.side = Side::both;
    }
    if (args.has("php_validate"))
        job.php_validate = to_bool(args.get("php_validate"));
    if (args.has("allow_different_virtual"))
        job.allow_different_virtual = to_bool(args.get("allow_different_virtual"));
    if (args.has("test_script"))
        job.test_script = args.get("test_script");
    if (args.has("test_script_args"))
        job.test_script_args = args.get("test_script_args");

    // Генерация: булевы
    if (args.has("generate_tests"))
        job.generate_tests = to_bool(args.get("generate_tests"));
    if (args.has("generate_intrusive"))
        job.generate_intrusive = to_bool(args.get("generate_intrusive"));
    if (args.has("generate_factory"))
        job.generate_factory = to_bool(args.get("generate_factory"));
    if (args.has("join_to_one_file"))
        job.join_to_one_file = to_bool(args.get("join_to_one_file"));
    if (args.has("auto_registration"))
        job.auto_registration = to_bool(args.get("auto_registration"));
    if (args.has("generate_ref_counter"))
        job.generate_ref_counter = to_bool(args.get("generate_ref_counter"));
    if (args.has("user_includes"))
        job.user_includes = to_bool(args.get("user_includes"));
    if (args.has("empty_methods"))
        job.empty_methods = to_bool(args.get("empty_methods"));

    auto raw = args.getAll("serialize_protocol");
    std::vector<std::string> tokens;
    for (auto &v : raw) {
        size_t start = 0;
        size_t pos = 0;
        while ((pos = v.find(',', start)) != std::string::npos) {
            tokens.push_back(v.substr(start, pos - start));
            start = pos + 1;
        }
        if (start < v.size())
            tokens.push_back(v.substr(start));
    }
    if (!tokens.empty()) {
        int mask = 0;
        std::vector<SerializeFormat> protos;
        for (auto &t : tokens) {
            std::string s = t;
            for(char& ch : s)
                ch = static_cast<char>(::tolower(static_cast<unsigned char>(ch)));
            if (s == "xml") {
                mask |= static_cast<int>(SerializeFormat::Xml);
                protos.push_back(SerializeFormat::Xml);
            }
            else if (s == "json") {
                mask |= static_cast<int>(SerializeFormat::Json);
                protos.push_back(SerializeFormat::Json);
            }
        }
        if (mask != 0) {
            job.serializeFormats = mask;
            job.serialize_protocol = std::move(protos);
        }
    }

    // Фильтры
    if (auto f = make_re2_filter_from_patterns(split_comma_allow_lists(args.getAll("filter_code"))); f)
        job.filter_code = std::move(f);
    if (auto f = make_re2_filter_from_patterns(split_comma_allow_lists(args.getAll("filter_data"))); f)
        job.filter_data = std::move(f);

    // Кастомный генератор
    if (args.has("custom_generator")) {
        std::string v = args.get("custom_generator");
        for(char& ch : v)
            ch = static_cast<char>(::tolower(static_cast<unsigned char>(ch)));
        if (v == "none")
            app.get_model().custom_generator = nullptr;
    }

    // Заполняем модель
    configuration.jobs.push_back(job);
    configuration.initialize();
    app.get_model().configuration = configuration;
    app.get_model().config = job;
    return true;
}

int main(int argc, char** argv) {
//    tests::run();
    
    auto start = std::chrono::steady_clock::now();
    
    Mlc app;
    {
        // Значения по умолчанию для локального прогона (могут быть переопределены флагами)
        Cli::ArgParser args;
        confugure_args(args, argc, argv);

        // Попытка загрузки параметров из конфига
        if(try_load_config(app, args)){
            
        } else if(try_load_argc(app, args)){
            
        }

        // Выполнение
        bool watch = args.has("watch");
        if (watch) {
            app.watchAndServe(/*poll_ms*/300, /*debounce_ms*/200);
        } else {
            if (app.get_model().config.only_data) {
                app.generateData();
            } else {
                app.generate();
                app.generateData();
            }
        }
    }
    
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    std::cout << "parsed on " << elapsed.count() << "ms" << std::endl;

    return 0;
}
