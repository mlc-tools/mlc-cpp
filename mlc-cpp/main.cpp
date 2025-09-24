#include <iostream>
#include <fstream>
#include "models/Model.hpp"
#include "core/Parser.hpp"
#include "core/Linker.hpp"
#include "utils/FileUtils.hpp"
#include "tests/tests.hpp"
#include "Mlc.hpp"

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
        app.setSide(Side::client);
        app.setGenerateTests(false);
        app.setGenerateIntrusive(false);
        app.addConfigsDirectory("/Users/vladimirtolmachev/work/mlc-cpp/tests/archero/configs");
        app.addDataDirectory("/Users/vladimirtolmachev/work/mlc-cpp/tests/archero/configs/data");
        app.setFilterCode([](const std::string& path){
            return path.find("/unit_tests/") == std::string::npos;
        });
        app.setOutDirectory("/Users/vladimirtolmachev/work/mlc-cpp/tests/archero/mg/");
        app.setOutDataDirectory("/Users/vladimirtolmachev/work/mlc-cpp/tests/archero/");
        // Проверим, нужно ли работать в режиме наблюдения
        bool watch = false;
        for (int i = 1; i < argc; ++i) {
            std::string a = argv[i];
            if (a == "--watch" || a == "-w") watch = true;
        }
        if (watch) {
            app.watchAndServe(/*poll_ms*/300, /*debounce_ms*/200);
        } else {
            app.generate();
            app.generateData();
        }
    }
//    {
//        app.setSide(Side::client);
//        app.setGenerateTests(false);
//        app.setGenerateIntrusive(false);
//        app.addConfigsDirectory("/Users/vladimirtolmachev/work/archero/configs/");
//        app.addDataDirectory("/Users/vladimirtolmachev/work/archero/configs/data/");
//        app.setFilterCode([](const std::string& path){
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
