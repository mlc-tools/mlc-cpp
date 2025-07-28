#include "parser_.hpp"
#include <iostream>
#include <fstream>
#include "Model.hpp"
#include "Parser.hpp"
#include "Linker.hpp"
#include "FileUtils.hpp"

std::string getFileContent(const std::string& path)
{
    std::fstream stream(path, std::ios::in);
    if (stream.is_open() == false)
        return "";
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
}

int main() {
    
    auto files = FileUtils::listFilesRecursive("config", {".mlc"});
    
    Model model;
    model.create_std_types();
    Parser parser(model);
    parser.parseFiles(files);
    
//    Linker().link(model);

//    auto classes = parse_class(code);
    auto cls = model.classes.at(0);
    
    std::cout << "Group: " << cls->group << "\n";
    std::cout << "Class: " << cls->name << "\n";
    std::cout << "Includes:\n";
    for (auto &i : cls->includes) std::cout << "  " << i << "\n";
    std::cout << "Members:\n";
    for (auto &m : cls->members) std::cout << "  " << m.type << " " << m.name << "\n";
    std::cout << "Methods:\n";
    for (auto &m : cls->functions) {
        std::cout << "  " << m.return_type.type << " " << m.name << "(";
        for (auto &a : m.args) std::cout << a.type << " " << a.name << " ";
        std::cout << ")\n";
    }
    std::cout << "Constructors:\n";
    for (auto &m : cls->constructors) {
        std::cout << "  " << m.name << "(";
        for (auto &a : m.args) std::cout << a.type << " " << a.name << " ";
        std::cout << ")\n";
    }
}
