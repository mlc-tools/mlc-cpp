//
//  Mlc.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#include "Mlc.hpp"

#include "CircularReference.hpp"
#include "Registrar.hpp"
//#include "Validator.hpp"
//#include "DataParser.hpp"
//#include "Language.hpp"
//#include "Log.hpp"
//#include "Error.hpp"
#include "FileUtils.hpp"
#include "module_cpp/GeneratorCpp.hpp"
#include "WriterCpp.hpp"
#include "SavePluginCpp.hpp"
#include "SerializerCpp.hpp"
#include "TranslatorCpp.hpp"

Mlc::Mlc(bool use_colors, bool disable_logs)
  : _filter_code(nullptr)
  , _filter_data(nullptr)
{
//    Log::useColors = use_colors;
//    Log::disableLogs = disable_logs;
    // _model сконструирован с дефолтами
}

// --- Сеттеры конфигурации (пример для пары) ---
void Mlc::addConfigsDirectory(const std::string &dir) {
    _model.configs_directories.push_back(FileUtils::normalizePath(dir));
}
void Mlc::setOutDirectory(const std::string &dir) {_model.out_directory = dir; }
void Mlc::addDataDirectory(const std::string &dir) {
    _model.data_directories.push_back(dir);
}
void Mlc::setOutDataDirectory(const std::string &dir) {_model.out_data_directory = dir; }
void Mlc::setLanguage(const std::string &lang) {_model.language = lang; }
void Mlc::setOnlyData(bool only) {_model.onlyData = only; }
void Mlc::setNamespace(const std::string &ns) {_model.namespaceName = ns; }
void Mlc::setSide(const Side &side) {_model.side = side; }
void Mlc::setPhpValidate(bool v) {_model.phpValidate = v; }
//void Mlc::setValidateAllowDifferentVirtualMethod(bool v) {_model.val = v; }
void Mlc::setTestScript(const std::string &script, const std::string &args) {
//TODO: impl
}
void Mlc::setGenerateTests(bool gen) {_model.generateTests = gen; }
void Mlc::setGenerateIntrusive(bool gen) {_model.generateIntrusive = gen; }
void Mlc::setGenerateFactory(bool gen) {_model.generateFactory = gen; }
void Mlc::setJoinToOneFile(bool j) {_model.joinToOneFile = j; }
void Mlc::setAutoRegistration(bool ar) {_model.auto_registration = ar; }
void Mlc::setGenerateRefCounter(bool grc) {_model.generateRefCounter = grc; }
void Mlc::setUserIncludes(bool ui) {_model.userIncludes = ui; }
void Mlc::setEmptyMethods(bool em) {_model.emptyMethods = em; }
void Mlc::setSerializeFormats(int formats) {_model.serializeFormats = formats; }
void Mlc::setFilterCode(std::function<bool(const std::string &)> f) {
    _filter_code = std::move(f);
}
void Mlc::setFilterData(std::function<bool(const std::string &)> f) {
    _filter_data = std::move(f);
}

void Mlc::setUserGenerator(std::shared_ptr<CustomGenerator> gen) {
    _custom_generator = std::move(gen);
}

// --- generate() ---
void Mlc::generate() {
    // 1. Очистка старых данных
    _model.clearData();

    // 2. Собираем все .mlc файлы из configs_directory и дополнительных
    std::vector<std::string> mlc_files;
    for (auto &dir : _model.configs_directories) {
        auto files = FileUtils::listFilesRecursive(dir, {".mlc"});  // возвращает имена внутри dir
        for (auto &f : files) {
            if (_filter_code && !_filter_code(f)) continue;
            mlc_files.push_back(f);
        }
    }

    // 3. Парсинг
    auto parser = std::make_shared<Parser>(_model);
    _model.parser = parser;
    parser->parseFiles(mlc_files);
    for(auto& cls : _model.classes){
        _model.classesDict[cls->name] = cls;
    }

    // 4. Генерация кода
    GeneratorCpp().generate(_model);

    // 5. Линковка и валидация
    Linker().link(_model);
//    Validator().validate(_model);

    // 6. Пользовательский генератор
//    runUserGeneratorInternal();

    // 7. Регистрация: создаём Registrar.h/.cpp только когда авто-регистрация выключена
    Registrar().generate(_model);

    // 8. Проверка циклических ссылок
    CircularReference(_model).find();

    // 9. Трансляция, сериализация и запись
    TranslatorCpp().translate(_model);
    SerializerCpp().generateMethods(_model);
    WriterCpp().save(_model);
    SavePluginCpp(_model).save_files(_model.joinToOneFile);
}

// --- generateData() ---
void Mlc::generateData() {
//    // Составляем список storage-классов
//    std::vector<std::shared_ptr<Class>> storage;
//    for (auto &cls : _model.classes)
//        if (cls->isStorage) storage.push_back(cls);
//    for (auto &cls : _model.classesForData)
//        if (cls->isStorage) storage.push_back(cls);
//
//    DataParser dataParser(storage,
//                          _model.dataDirectory,
//                          _filter_data);
//    dataParser.parse(_model.additionalDataDirectories);
//    dataParser.flush(_model.out_data_directory);
}

// --- runTest() ---
void Mlc::runTest() {
//    const auto &script = _model.testScript;
//    if (script.empty() || !std::filesystem::exists(script)) {
//        Log::error("Test script (" + script + ") not found");
//        std::exit(1);
//    }
//    std::string cmd = std::string(
//        (PY_VERSION_MAJOR >= 3 ? "python3" : "python")) +
//        " " + script + " " + _model.testScriptArgs;
//    Log::message("Run test (" + cmd + "):");
//    int rc = std::system(cmd.c_str());
//    if (rc != 0) {
//        Error::exit(Error::TESTS_FAILED);
//    }
}

// --- Внутренний запуск пользовательского генератора ---
void Mlc::runUserGeneratorInternal() {
//    if (_custom_generator) {
//        _custom_generator->execute(_model);
//    }
}
