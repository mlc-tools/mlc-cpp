#include <iostream>
#include <fstream>
#include "models/Model.hpp"
#include "core/Parser.hpp"
#include "core/Linker.hpp"
#include "utils/FileUtils.hpp"
#include "tests/tests.hpp"

std::string getFileContent(const std::string& path)
{
    std::fstream stream(path, std::ios::in);
    if (stream.is_open() == false)
        return "";
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
}

int main() {
    tests::run();
    
//    auto files = FileUtils::listFilesRecursive("/Users/vladimirtolmachev/work/dungeon2/configs", {".mlc"});
    auto files = FileUtils::listFilesRecursive("/Users/vladimirtolmachev/work/archero/configs", {".mlc"});
//    auto files = FileUtils::listFilesRecursive("configs", {".mlc"});
    
    auto start = std::chrono::steady_clock::now();
    Model model;
    model.create_std_types();
    Parser parser(model);
    parser.parseFiles(files);
    
//    Linker().link(model);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
    std::cout << "parsed on " << elapsed.count() << "ms";

//    auto classes = parse_class(code);
//    auto cls = model.classes.at(0);
//    
//    std::cout << "Group: " << cls->group << "\n";
//    std::cout << "Class: " << cls->name << "\n";
//    std::cout << "Includes:\n";
//    for (auto &i : cls->includes) std::cout << "  " << i << "\n";
//    std::cout << "Members:\n";
//    for (auto &m : cls->members) std::cout << "  " << m.type << " " << m.name << "\n";
//    std::cout << "Methods:\n";
//    for (auto &m : cls->functions) {
//        std::cout << "  " << m.return_type.type << " " << m.name << "(";
//        for (auto &a : m.callable_args) std::cout << a.type << " " << a.name << " ";
//        std::cout << ")\n";
//    }
//    std::cout << "Constructors:\n";
//    for (auto &m : cls->constructors) {
//        std::cout << "  " << m.name << "(";
//        for (auto &a : m.callable_args) std::cout << a.type << " " << a.name << " ";
//        std::cout << ")\n";
//    }
}
