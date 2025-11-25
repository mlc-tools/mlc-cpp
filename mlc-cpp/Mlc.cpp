//
//  Mlc.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#include "Mlc.hpp"

#include "CircularReference.hpp"
#include "Registrar.hpp"
// #include "Validator.hpp"
#include "features/DataParser.hpp"
// #include "Language.hpp"
#include "Error.hpp"
#include "FileUtils.hpp"
#include "SavePluginCpp.hpp"
#include "SerializerCpp.hpp"
#include "TranslatorCpp.hpp"
#include "WriterCpp.hpp"
#include "utils/Config.hpp"
#include "features/GeneratorBindings.hpp"
#include "features/GeneratorEcsCpp.hpp"
#include "features/GeneratorUnitTestsInterface.hpp"
#include "features/GeneratorVisitor.hpp"
#include "features/cpp/GeneratorDataStorageCpp.hpp"
#include "features/cpp/GeneratorOperatorEqualsCpp.hpp"
#include "features/cpp/GeneratorRefCounterCpp.hpp"
#include "features/py/GeneratorDataStoragePython.hpp"
#include "features/py/GeneratorOperatorEqualsPython.hpp"
#include "module_cpp/GeneratorCpp.hpp"
#include "module_python/GeneratorPython.hpp"
#include "module_python/SavePluginPython.hpp"
#include "module_python/SerializerPython.hpp"
#include "module_python/TranslatorPython.hpp"
#include "module_python/WriterPython.hpp"

#include <atomic>
#include <csignal>
#include <functional>
#include <iostream>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace {

static std::atomic<bool> g_stop{false};
static std::atomic<bool> g_force_reload{false};

void handle_sigint(int) { g_stop = true; }
void handle_sighup(int) { g_force_reload = true; }

} // namespace

Mlc::Mlc(bool use_colors, bool disable_logs) {
    Log::use_colors = use_colors;
    Log::disable_logs = disable_logs;
    // _model сконструирован с дефолтами
}

// --- generate() ---
void Mlc::generate() {
    // 1. Очистка старых данных

    for (auto &job : _model.configuration.jobs) {
        _model.config = job;
        _model.clearData();

        // 2. Собираем все .mlc файлы из configs_directory и дополнительных
        std::vector<std::string> mlc_files;
        for (auto &dir : _model.configuration.configs_directories) {
            auto files = FileUtils::listFilesRecursive(
                dir, {".mlc"}); // возвращает имена внутри dir
            for (auto &f : files) {
                if (_model.config.filter_code && !_model.config.filter_code(f))
                    continue;
                mlc_files.push_back(f);
            }
        }

        // 3. Парсинг
        auto parser = std::make_shared<Parser>(_model);
        _model.parser = parser;
        parser->parseFiles(mlc_files);
        for (auto &cls : _model.classes) {
            _model.classesDict[cls->name] = cls;
        }

        // 6. Генераторы фич
        buildFeatureGenerators();
        for (auto &gen : _model.feature_generators)
            gen->generate(_model);

        // 4. Генерация кода
        if (_model.config.language == "py") {
            GeneratorPython().generate(_model);
        } else {
            GeneratorCpp().generate(_model);
        }

        // 5. Линковка и валидация
        Linker().link(_model);
        //    Validator().validate(_model);

        // 7. Регистрация: создаём Registrar.h/.cpp только когда
        // авто-регистрация выключена
        Registrar().generate(_model);

        // 8. Проверка циклических ссылок
        CircularReference(_model).find();

        // 9. Трансляция, сериализация и запись
        if (_model.config.language == "py") {
            TranslatorPython().translate(_model);
            SerializerPython().generateMethods(_model);
            WriterPython().save(_model);
            SavePluginPython save(_model, _model.configuration.get_feature<FeatureUnityFile>());
            save.save_files();
            save.removeOldFiles();
        } else {
            TranslatorCpp().translate(_model);
            SerializerCpp().generateMethods(_model);
            WriterCpp().save(_model);
            SavePluginCpp save(_model, _model.configuration.get_feature<FeatureUnityFile>());
            save.save_files();
            save.removeOldFiles();
        }
    }
}

// --- generateData() ---
void Mlc::generateData() {
    // Составляем список storage-классов
    std::vector<std::shared_ptr<Class>> storage;
    for (auto &cls : _model.classes)
        if (cls->is_storage)
            storage.push_back(cls);
    for (auto &cls : _model.classesForData)
        if (cls->is_storage)
            storage.push_back(cls);

    for (auto &job : _model.configuration.jobs) {
        _model.config = job;
        if (_model.config.out_data_directory.empty())
            continue;
        DataParser dataParser(_model, storage, _model.config.filter_data);
        dataParser.parse(_model.configuration.data_directories);
        dataParser.flush(_model.config.out_data_directory);
    }
}

// --- runTest() ---
void Mlc::runTest() {
    //    const auto &script = _model.test_script;
    //    if (script.empty() || !std::filesystem::exists(script)) {
    //        Log::error("Test script (" + script + ") not found");
    //        std::exit(1);
    //    }
    //    std::string cmd = std::string(
    //        (PY_VERSION_MAJOR >= 3 ? "python3" : "py")) +
    //        " " + script + " " + _model.test_script_args;
    //    Log::message("Run test (" + cmd + "):");
    //    int rc = std::system(cmd.c_str());
    //    if (rc != 0) {
    //        Error::exit(Error::TESTS_FAILED);
    //    }
}

void Mlc::buildFeatureGenerators() {
    _model.feature_generators.clear();
    for (const auto &feature : _model.configuration.features) {
        auto gen = std::visit(
            [&](auto &&arg) -> std::shared_ptr<FeatureGenerator> {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, FeatureEcs>)
                    return std::make_shared<GeneratorEcsCpp>(arg);
                else if constexpr (std::is_same_v<T, FeatureVisitor>)
                    return std::make_shared<GeneratorVisitor>();
                else if constexpr (std::is_same_v<T, FeatureUnitTests>)
                    return std::make_shared<GeneratorUnitTestsInterface>();
                else if constexpr (std::is_same_v<T, FeatureDataStorage>) {
                    if (_model.config.language == "cpp")
                        return std::make_shared<GeneratorDataStorageCpp>(arg);
                    if (_model.config.language == "py")
                        return std::make_shared<GeneratorDataStoragePython>(arg);
                    return nullptr;
                } else if constexpr (std::is_same_v<T, FeatureRefCounter>)
                    return std::make_shared<GeneratorRefCounterCpp>();
                else if constexpr (std::is_same_v<T, FeatureOperatorEquals>) {
                    if (_model.config.language == "cpp")
                        return std::make_shared<GeneratorOperatorEqualsCpp>();
                    if (_model.config.language == "py")
                        return std::make_shared<
                        GeneratorOperatorEqualsPython>();
                    return nullptr;
                }
                else if constexpr (std::is_same_v<T, FeatureBindings>)
                    return std::make_shared<GeneratorBindings>(arg);
                return nullptr;
            },
            feature);
        if (gen)
            _model.feature_generators.push_back(std::move(gen));
    }
}

void Mlc::watchAndServe(unsigned poll_ms, unsigned debounce_ms) {
    // Устанавливаем обработчики сигналов для аккуратного выхода/перезагрузки
    std::signal(SIGINT, handle_sigint);
#ifdef SIGHUP
    std::signal(SIGHUP, handle_sighup);
#endif

    // Первичная генерация
    try {
        generate();
    } catch (const std::exception &e) {
        Log::error(std::string("Initial generation failed: ") + e.what());
    }

    // Снимки mtimes для .mlc и данных
    const std::vector<std::string> code_exts = {".mlc"};
    // Для данных следим за всем (xml,json, и пр.), можно ограничить при желании
    const std::vector<std::string> data_exts = {}; // все файлы
    FileUtils::Snapshot snap_code = FileUtils::scan_dirs(
        _model.configuration.configs_directories, code_exts);
    FileUtils::Snapshot snap_data =
        FileUtils::scan_dirs(_model.configuration.data_directories, data_exts);

    Log::message("Watching for changes. Press Ctrl+C to stop...");

    while (!g_stop.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(poll_ms));

        bool changed_code = false;
        bool changed_data = false;

        // Пересканируем и сравним
        FileUtils::Snapshot cur_code = FileUtils::scan_dirs(
            _model.configuration.configs_directories, code_exts);
        FileUtils::Snapshot cur_data = FileUtils::scan_dirs(
            _model.configuration.data_directories, data_exts);

        if (g_force_reload.exchange(false)) {
            changed_code = true; // принудительно перегенерить всё
            changed_data = true;
        } else {
            changed_code = has_changes(snap_code, cur_code);
            changed_data = has_changes(snap_data, cur_data);
        }

        if (changed_code || changed_data) {
            // Небольшой дебаунс, чтобы поймать пачки изменений
            std::this_thread::sleep_for(std::chrono::milliseconds(debounce_ms));

            try {
                if (changed_code) {
                    std::vector<std::string> changedFiles;
                    std::vector<std::string> removedFiles;
                    diff_snapshots(snap_code, cur_code, changedFiles,
                                   removedFiles);
                    generateIncremental(changedFiles, removedFiles);
                    generateData();
                } else if (changed_data) {
                    generateData();
                }
                snap_code = cur_code;
                snap_data = cur_data;
            } catch (const std::exception &e) {
                Log::error(std::string("Regeneration failed: ") + e.what());
            }
        }
    }
    Log::message("Stopping watch loop.");
}

void Mlc::generateIncremental(const std::vector<std::string> &changedFiles,
                              const std::vector<std::string> &removedFiles) {
    PROFILE_START(0, "Incremental generation start");
    // 1) Удалить классы из удалённых .mlc и собрать имена для удаления файлов

    for (auto &job : _model.configuration.jobs) {
        _model.config = job;
        std::vector<std::string> removedClassNames;
        std::vector<std::pair<std::string, std::string>> removedNameGroup;
        std::vector<std::pair<std::string, std::string>> changedNameGroup;
        for (const auto &rf : removedFiles) {
            _model.remove_classes_from_source(rf, &removedClassNames,
                                              &removedNameGroup);
        }
        std::vector<std::string> changedClassNames;
        for (const auto &cf : changedFiles) {
            _model.remove_classes_from_source(cf, &changedClassNames,
                                              &changedNameGroup);
        }
        removedClassNames.insert(removedClassNames.end(),
                                 changedClassNames.begin(),
                                 changedClassNames.end());
        removedNameGroup.insert(removedNameGroup.end(),
                                changedNameGroup.begin(),
                                changedNameGroup.end());

        // 3) Распарсить изменённые файлы заново
        if (!changedFiles.empty()) {
            if (!_model.parser)
                _model.parser = std::make_shared<Parser>(_model);
            _model.parser->parseFiles(changedFiles);
        }

        // 5) Построить множество грязных классов: изменённые + их все подклассы
        // транзитивно
        std::unordered_set<std::string> dirty;
        for (const auto &name : changedClassNames)
            dirty.insert(name);
        for (const auto &cf : changedFiles) {
            auto it = _model.source_to_classnames.find(cf);
            if (it != _model.source_to_classnames.end()) {
                for (const auto &n : it->second)
                    dirty.insert(n);
            }
        }
        // транзитивно добавим всех наследников
        std::function<void(const std::shared_ptr<Class> &)> add_subs;
        add_subs = [&](const std::shared_ptr<Class> &c) {
            for (auto &w : c->subclasses) {
                if (auto s = w.lock()) {
                    if (dirty.insert(s->name).second)
                        add_subs(s);
                }
            }
        };
        for (const auto &n :
             std::vector<std::string>(dirty.begin(), dirty.end())) {
            auto c = _model.get_class(n);
            if (c)
                add_subs(c);
        }
        _model.dirty_classes = std::move(dirty);

        for (const auto &[name, group] : removedNameGroup) {
            if (_model.hasClass(name))
                continue;
            auto local_h =
                (group.empty() ? name + ".h" : group + "/" + name + ".h");
            auto local_cpp =
                (group.empty() ? name + ".cpp" : group + "/" + name + ".cpp");
            std::string full_h =
                FileUtils::normalizePath(_model.config.out_directory) + local_h;
            std::string full_cpp =
                FileUtils::normalizePath(_model.config.out_directory) +
                local_cpp;
            if (FileUtils::exists(full_h)) {
                Log::message(" Removed:     " + local_h);
                FileUtils::remove(full_h);
            }
            if (FileUtils::exists(full_cpp)) {
                Log::message(" Removed:     " + local_cpp);
                FileUtils::remove(full_cpp);
            }
        }

        _model.files.clear();
        _model.created_files.clear();
        if (_model.config.language == "py") {
            GeneratorPython().generate(_model);
        } else {
            GeneratorCpp().generate(_model);
        }

        for (auto &gen : _model.feature_generators)
            gen->generate(_model);
        Linker().link(_model);

        Registrar().generate(_model);

        if (!_model.dirty_classes.empty()) {
            if (_model.config.language == "py") {
                TranslatorPython().translate(_model);
                SerializerPython().generateMethods(_model);
                WriterPython().save(_model);
            } else {
                TranslatorCpp().translate(_model);
                SerializerCpp().generateMethods(_model);
                WriterCpp().save(_model);
            }
        }
        
        auto& feature = _model.configuration.get_feature<FeatureUnityFile>();
        if (_model.config.language == "py") {
            SavePluginPython(_model, feature).save_files();
        } else {
            SavePluginCpp(_model, feature).save_files();
        }

        _model.dirty_classes.clear();
    }
    PROFILE_STEP(1, "Incremental generation finished");
}
